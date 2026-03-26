#pragma once

#include "../java.h"
#include "Abilities.h"
#include <memory>


class Player : public JavaObject {
public:
	Player(jobject obj);
	std::unique_ptr<Abilities> getAbilities() const;

private:
	static void init();
	static jclass clazz;
	static jfieldID abilities;
};