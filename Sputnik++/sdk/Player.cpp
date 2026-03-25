#include "Player.h"

jclass Player::clazz = nullptr;
jfieldID Player::abilities = nullptr;

void Player::init() {
    if (clazz) return;
    jclass cls = Java::env->FindClass("net/minecraft/client/player/LocalPlayer");
    clazz = (jclass)Java::env->NewGlobalRef(cls);
    Java::env->DeleteLocalRef(cls);
    abilities = Java::env->GetFieldID(clazz, "abilities", "Lnet/minecraft/world/entity/player/Abilities;");
}

Player::Player(jobject obj) : JavaObject(obj) {
    init();
}

std::unique_ptr<Abilities> Player::getAbilities() const {
    jobject ab = Java::env->GetObjectField(obj, abilities);
    if (!ab) return nullptr;
    return std::make_unique<Abilities>(ab);
}