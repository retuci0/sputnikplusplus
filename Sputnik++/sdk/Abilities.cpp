#include "Abilities.h"
#include "classnames.h"


jclass   Abilities::clazz = nullptr;
jfieldID Abilities::mayflyField = nullptr;
jfieldID Abilities::flyingField = nullptr;

bool Abilities::init() {
    if (clazz) return true;
    clazz = Java::findClass(Classes::ABILITIES);
    if (!clazz) return false;

    JNIEnv* env = Java::getEnv();
    mayflyField = env->GetFieldID(clazz, "mayfly", "Z");
    flyingField = env->GetFieldID(clazz, "flying", "Z");

    if (env->ExceptionCheck() || !mayflyField || !flyingField) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        clazz = nullptr;
        return false;
    }
    return true;
}

Abilities::Abilities(jobject local) : JavaObject(local) {
    init();
}

void Abilities::setMayfly(bool value) {
    if (!isValid()) return;
    Java::getEnv()->SetBooleanField(obj, mayflyField, value);
}

void Abilities::setFlying(bool value) {
    if (!isValid()) return;
    Java::getEnv()->SetBooleanField(obj, flyingField, value);
}

bool Abilities::mayFly() const {
    if (!isValid()) return false;
    return Java::getEnv()->GetBooleanField(obj, mayflyField);
}

bool Abilities::isFlying() const {
    if (!isValid()) return false;
    return Java::getEnv()->GetBooleanField(obj, flyingField);
}