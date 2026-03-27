#include "Minecraft.h"
#include <iostream>

jclass MinecraftClient::clazz = nullptr;
jfieldID MinecraftClient::instance = nullptr;
jfieldID MinecraftClient::player = nullptr;

void MinecraftClient::init() {
	if (clazz) return;
	clazz = Java::findClass("gfj");

	instance = Java::getEnv()->GetStaticFieldID(clazz, "A", "Lgfj;");
	player = Java::getEnv()->GetFieldID(clazz, "s", "Lddm;");
}

MinecraftClient::MinecraftClient() : JavaObject(nullptr) {
	init();
	jobject mc = Java::getEnv()->GetStaticObjectField(clazz, instance);
	obj = Java::getEnv()->NewGlobalRef(mc);
	Java::getEnv()->DeleteLocalRef(mc);
}

std::unique_ptr<Player> MinecraftClient::getPlayer() const {
	return Java::getField<Player>(raw(), "s", "Lddm;");
}