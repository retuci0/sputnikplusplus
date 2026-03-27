#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <jni.h>
#include <string>
#include <windows.h>


struct Java {
    static inline JavaVM* jvm = nullptr;

    static JNIEnv* getEnv() {
        JNIEnv* env = nullptr;
        if (jvm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
            jvm->AttachCurrentThread((void**)&env, nullptr);
        }
        return env;
    }

    static jclass findClass(const char* name) {
        JNIEnv* env = getEnv();
        jclass cls = env->FindClass(name);
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            return nullptr;
        }
        if (!cls) return nullptr;
        jclass global = (jclass)env->NewGlobalRef(cls);
        env->DeleteLocalRef(cls);
        return global;
    }

    template<typename T>
    static std::unique_ptr<T> getField(jobject obj, const char* name, const char* sig) {
        if (!obj) {
            std::cerr << "obj was null";
            return nullptr;
        }
        JNIEnv* env = getEnv();
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, name, sig);
        if (env->ExceptionCheck() || !fid) {
            env->ExceptionClear();
            std::cerr << "fid was null";
            return nullptr;
        }
        jobject fieldObj = env->GetObjectField(obj, fid);
        if (env->ExceptionCheck() || !fieldObj) {
            env->ExceptionClear();
            std::cerr << "fieldObj was null";
            return nullptr;
        }
        auto result = std::make_unique<T>(fieldObj);
        env->DeleteLocalRef(fieldObj);
        env->DeleteLocalRef(cls);
        return result;
    }
};

static inline JavaVM* getJVM() {
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

static inline JNIEnv* getJNIEnv() {
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
        : obj(Java::getEnv()->NewGlobalRef(obj)) {}

    virtual ~JavaObject() {
        if (obj) {
            Java::getEnv()->DeleteGlobalRef(obj);
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
            if (obj) Java::getEnv()->DeleteGlobalRef(obj);
            obj = other.obj;
            other.obj = nullptr;
        }
        return *this;
    }

protected:
    jobject obj;
    jobject raw() const { return obj; }
};