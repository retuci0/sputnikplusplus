#pragma once

#include "../java.h"


class Abilities : public JavaObject {
public:
    explicit Abilities(jobject local);

    void setMayfly(bool value);
    void setFlying(bool value);
    bool mayFly() const;
    bool isFlying() const;

private:
    static bool init();

    static jclass clazz;
    static jfieldID mayflyField;
    static jfieldID flyingField;
};
