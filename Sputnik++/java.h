#pragma once

#include <cassert>
#include <jni.h>
#include <string>
#include <windows.h>


struct Java {
    static inline JavaVM* jvm = nullptr;
    static inline JNIEnv* env = nullptr;
};

JavaVM* getJVM() {
    HMODULE jvmModule = GetModuleHandleA("jvm.dll");
    if (!jvmModule) return nullptr;

    using JNI_GetCreatedJavaVMs_t = jint(JNICALL*)(JavaVM**, jsize, jsize*);
    auto JNI_GetCreatedJavaVMs = reinterpret_cast<JNI_GetCreatedJavaVMs_t>(
        GetProcAddress(jvmModule, "JNI_GetCreatedJavaVMs"));
    if (!JNI_GetCreatedJavaVMs) return nullptr;

    jsize count = 0;
    JavaVM* vm;
    jint result = JNI_GetCreatedJavaVMs(&vm, 1, &count);
    if (result == JNI_OK && count > 0 && vm != nullptr) return vm;
    return nullptr;
}

JNIEnv* getJNIEnv() {
    if (!Java::jvm) return nullptr;

    JNIEnv* env = nullptr;
    jint ret = Java::jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);

    if (ret == JNI_EDETACHED) {
        char name[] = "sputnik-thread";
        JavaVMAttachArgs args = { JNI_VERSION_1_8, name, nullptr};
        ret = Java::jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), &args);
        if (ret != JNI_OK) return nullptr;
    }
    return env;
}


// wrapper de jobject no copiable pero amovible
class JavaObject {
public:
    JavaObject(jobject obj)
        : obj(Java::env->NewGlobalRef(obj)) {
        Java::env->DeleteLocalRef(obj);
    }

    virtual ~JavaObject() {
        if (obj) {
            Java::env->DeleteGlobalRef(obj);
        }
    }

    JavaObject(const JavaObject&) = delete;
    JavaObject& operator=(const JavaObject&) = delete;

    JavaObject(JavaObject&& other) noexcept
        : obj(other.obj) {
        other.obj = nullptr;
    }

    JavaObject& operator=(JavaObject&& other) noexcept {
        if (this != &other) {
            if (obj) Java::env->DeleteGlobalRef(obj);
            obj = other.obj;
            other.obj = nullptr;
        }
        return *this;
    }

protected:
    jobject obj;

    static jmethodID getMethod(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
        jmethodID mid = env->GetMethodID(clazz, name, sig);
        assert(mid);
        return mid;
    }

    static jfieldID getField(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
        jfieldID fid = env->GetFieldID(clazz, name, sig);
        assert(fid);
        return fid;
    }

    jobject raw() const { return obj; }
};