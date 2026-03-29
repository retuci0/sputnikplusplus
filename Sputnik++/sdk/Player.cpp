#include "Player.h"
#include "classnames.h"


jclass Player::clazz = nullptr;
jfieldID Player::abilitiesField = nullptr;

bool Player::init() {
    if (clazz) return true;
    clazz = Java::findClass(Classes::PLAYER);
    if (!clazz) return false;

    JNIEnv* env = Java::getEnv();
    abilitiesField = env->GetFieldID(clazz, "abilities", Classes::sig(Classes::ABILITIES).c_str());

    if (env->ExceptionCheck() || !abilitiesField) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        clazz = nullptr;
        return false;
    }

    return true;
}

Player::Player(jobject local) : JavaObject(local) {
    init();
}

std::unique_ptr<Abilities> Player::getAbilities() const {
    return Java::getField<Abilities>(obj, abilitiesField);
}