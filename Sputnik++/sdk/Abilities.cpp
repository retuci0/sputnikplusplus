#include "Abilities.h"


jclass Abilities::clazz = nullptr;
jfieldID Abilities::mayfly = nullptr;
jfieldID Abilities::flying = nullptr;

void Abilities::init() {
    if (clazz) return;
    jclass cls = Java::env->FindClass("net/minecraft/world/entity/player/Abilities");
    clazz = (jclass)Java::env->NewGlobalRef(cls);
    Java::env->DeleteLocalRef(cls);

    mayfly = Java::env->GetFieldID(clazz, "mayfly", "Z");
    flying = Java::env->GetFieldID(clazz, "flying", "Z");
}

Abilities::Abilities(jobject obj) : JavaObject(obj) {
    init();
}

void Abilities::setMayfly(bool value) {
    Java::env->SetBooleanField(obj, mayfly, value);
}

void Abilities::setFlying(bool value) {
    Java::env->SetBooleanField(obj, flying, value);
}

bool Abilities::mayFly() const {
    return Java::env->GetBooleanField(obj, mayfly);
}

bool Abilities::isFlying() const {
    return Java::env->GetBooleanField(obj, flying);
}