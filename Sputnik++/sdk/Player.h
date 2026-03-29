#pragma once

#include "../java.h"
#include "Abilities.h"
#include <memory>


class Player : public JavaObject {
public:
    explicit Player(jobject local);

    std::unique_ptr<Abilities> getAbilities() const;

private:
    static bool init();

    static jclass clazz;
    static jfieldID abilitiesField;
};