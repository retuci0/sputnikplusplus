#include "Minecraft.h"
#include "classnames.h"

#include <iostream>


jclass MinecraftClient::clazz = nullptr;
jmethodID MinecraftClient::getInstanceMethod = nullptr;
jfieldID MinecraftClient::playerField = nullptr;

bool MinecraftClient::init() {
    if (clazz) return true;
    clazz = Java::findClass(Classes::MINECRAFT);
    if (!clazz) return false;

    JNIEnv* env = Java::getEnv();

    getInstanceMethod = env->GetStaticMethodID(clazz, "getInstance", "()Lnet/minecraft/client/Minecraft;");
    playerField = env->GetFieldID(clazz, "player", Classes::sig(Classes::PLAYER).c_str());

    if (env->ExceptionCheck() || !getInstanceMethod || !playerField) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        clazz = nullptr;
        return false;
    }

    return true;
}

std::unique_ptr<Player> MinecraftClient::getPlayer() {
    if (!init()) return nullptr;
    JNIEnv* env = Java::getEnv();

    jobject mcLocal = env->CallStaticObjectMethod(clazz, getInstanceMethod);
    if (env->ExceptionCheck() || !mcLocal) {
        env->ExceptionClear();
        return nullptr;
    }

    jobject playerLocal = env->GetObjectField(mcLocal, playerField);
    env->DeleteLocalRef(mcLocal);
    if (!playerLocal) return nullptr;

    auto player = std::make_unique<Player>(playerLocal);
    env->DeleteLocalRef(playerLocal);
    return player;
}