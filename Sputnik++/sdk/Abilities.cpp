#include "Abilities.h"


jclass Abilities::clazz = nullptr;
jfieldID Abilities::mayfly = nullptr;
jfieldID Abilities::flying = nullptr;

void Abilities::init() {
    if (clazz) return;
    clazz = Java::findClass("ddi");

    mayfly = Java::getEnv()->GetFieldID(clazz, "c", "Z");
    flying = Java::getEnv()->GetFieldID(clazz, "b", "Z");
}

Abilities::Abilities(jobject obj) : JavaObject(obj) {
    init();
}

void Abilities::setMayfly(bool value) {
    Java::getEnv()->SetBooleanField(obj, mayfly, value);
}

void Abilities::setFlying(bool value) {
    Java::getEnv()->SetBooleanField(obj, flying, value);
}

bool Abilities::mayFly() const {
    return Java::getEnv()->GetBooleanField(obj, mayfly);
}

bool Abilities::isFlying() const {
    return Java::getEnv()->GetBooleanField(obj, flying);
}