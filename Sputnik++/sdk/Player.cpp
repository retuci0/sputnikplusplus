#include "Player.h"
#include "Minecraft.h"

jclass Player::clazz = nullptr;
jfieldID Player::abilities = nullptr;

void Player::init() {
    if (clazz) return;
    clazz = Java::findClass("ddm");
    abilities = Java::getEnv()->GetFieldID(clazz, "cG", "Lddi;");
}

Player::Player(jobject obj) : JavaObject(obj) {
    init();
}

std::unique_ptr<Abilities> Player::getAbilities() const {
    return Java::getField<Abilities>(raw(), "cG", "Lddi;");
}