#include "Minecraft.h"


jclass MinecraftClient::clazz = nullptr;
jfieldID MinecraftClient::instance = nullptr;
jfieldID MinecraftClient::player = nullptr;

void MinecraftClient::init() {
	if (clazz) return;
	jclass cls = Java::env->FindClass("net/minecraft/client/Minecraft");
	clazz = reinterpret_cast<jclass>(Java::env->NewGlobalRef(cls));
	Java::env->DeleteLocalRef(cls);

	instance = Java::env->GetFieldID(clazz, "instance", "Lnet/minecraft/client/Minecraft;");
	player = Java::env->GetFieldID(clazz, "player", "Lnet/minecraft/client/player/LocalPlayer;");
}

MinecraftClient::MinecraftClient() : JavaObject(nullptr) {
	init();
	jobject mc = Java::env->GetStaticObjectField(clazz, instance);
	obj = Java::env->NewGlobalRef(mc);
	Java::env->DeleteLocalRef(mc);
}

std::unique_ptr<Player> MinecraftClient::getPlayer() const {
	jobject p = Java::env->GetObjectField(obj, player);
	if (!p) return nullptr;
	return std::make_unique<Player>(p);
}