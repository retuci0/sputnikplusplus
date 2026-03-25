#pragma once

#include "../Java.h"
#include <memory>

class Player;


class MinecraftClient : public JavaObject {
public:
	static MinecraftClient* getInstance() {
		static MinecraftClient INSTANCE;
		return &INSTANCE;
	}

	std::unique_ptr<Player> getPlayer() const;

private:
	MinecraftClient();
	static void init();

	static jclass clazz;
	static jfieldID instance;
	static jfieldID player;
};