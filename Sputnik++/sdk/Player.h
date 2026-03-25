#pragma once

#include "../java.h"
#include <memory>

class Abilities;


class Player : public JavaObject {
public:
	Player(jobject obj);
	std::unique_ptr<Abilities> getAbilities() const;

private:
	static void init();
	static jclass clazz;
	static jfieldID abilities;
};