#pragma once

#include <memory>
#include <jni.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif


struct Java {
    static inline JavaVM* jvm = nullptr;

	// obtiene el JNIEnv para el hilo actual, adjuntándolo a la JVM si es necesario
    static JNIEnv* getEnv() {
        if (!jvm) return nullptr;
        JNIEnv* env = nullptr;
        jint ret = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);
        if (ret == JNI_EDETACHED) {
            JavaVMAttachArgs args{ JNI_VERSION_1_8, (char*)"client-thread", nullptr };
            if (jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), &args) != JNI_OK)
                return nullptr;
        }
        return env;
    }

    static jclass findClass(const char* name) {
        JNIEnv* env = getEnv();
        if (!env) return nullptr;
        jclass local = env->FindClass(name);
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            return nullptr;
        }
        if (!local) return nullptr;
        auto global = static_cast<jclass>(env->NewGlobalRef(local));
        env->DeleteLocalRef(local);
        return global;
    }

    template<typename T>
    static std::unique_ptr<T> getField(jobject obj, jfieldID fid) {
        if (!obj || !fid) return nullptr;
        JNIEnv* env = getEnv();
        if (!env) return nullptr;
        jobject local = env->GetObjectField(obj, fid);
        if (env->ExceptionCheck() || !local) {
            env->ExceptionClear();
            return nullptr;
        }
        auto result = std::make_unique<T>(local);
        env->DeleteLocalRef(local);
        return result;
    }

    template<typename T>
    static std::unique_ptr<T> getStaticField(jclass cls, jfieldID fid) {
        if (!cls || !fid) return nullptr;
        JNIEnv* env = getEnv();
        if (!env) return nullptr;
        jobject local = env->GetStaticObjectField(cls, fid);
        if (env->ExceptionCheck() || !local) {
            env->ExceptionClear();
            return nullptr;
        }
        auto result = std::make_unique<T>(local);
        env->DeleteLocalRef(local);
        return result;
    }
};

static inline JavaVM* getJVM() {
#ifdef _WIN32
    HMODULE mod = GetModuleHandleA("jvm.dll");
    if (!mod) return nullptr;
    using Fn = jint(JNICALL*)(JavaVM**, jsize, jsize*);
    auto fn = reinterpret_cast<Fn>(GetProcAddress(mod, "JNI_GetCreatedJavaVMs"));
    if (!fn) return nullptr;
    JavaVM* vm = nullptr;
    jsize count = 0;
    return (fn(&vm, 1, &count) == JNI_OK && count > 0) ? vm : nullptr;
#else
    void* handle = dlopen(NULL, RTLD_LAZY);
    if (!handle) return nullptr;
    using Fn = jint(JNICALL*)(JavaVM**, jsize, jsize*);
    Fn fn = reinterpret_cast<Fn>(dlsym(handle, "JNI_GetCreatedJavaVMs"));
    if (!fn) {
        dlclose(handle);
        return nullptr;
    }
    JavaVM* vm = nullptr;
    jsize count = 0;
    jint res = fn(&vm, 1, &count);
    dlclose(handle);
    return (res == JNI_OK && count > 0) ? vm : nullptr;
#endif
}



// wrapper sobre una referencia global de JNI que no se puede copiar pero sí mover
class JavaObject {
public:
    explicit JavaObject(jobject local) : obj(nullptr) {
        if (local) {
            JNIEnv* env = Java::getEnv();
            if (env) obj = env->NewGlobalRef(local);
        }
    }

    virtual ~JavaObject() {
        if (obj) {
            JNIEnv* env = Java::getEnv();
            if (env) env->DeleteGlobalRef(obj);
        }
    }

    JavaObject(const JavaObject&) = delete;
    JavaObject& operator=(const JavaObject&) = delete;

    JavaObject(JavaObject&& other) noexcept : obj(other.obj) {
        other.obj = nullptr;
    }

    JavaObject& operator=(JavaObject&& other) noexcept {
        if (this != &other) {
            if (obj) {
                JNIEnv* env = Java::getEnv();
                if (env) env->DeleteGlobalRef(obj);
            }
            obj = other.obj;
            other.obj = nullptr;
        }
        return *this;
    }

    jobject raw() const { return obj; }
    bool isValid() const { return obj != nullptr; }

protected:
    jobject obj;
};