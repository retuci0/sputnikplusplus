#pragma once

#include "../java.h"


class Abilities : public JavaObject {
public:
    Abilities(jobject obj);
    void setMayfly(bool value);
    void setFlying(bool value);
    bool mayFly() const;
    bool isFlying() const;

private:
    static void init();
    static jclass clazz;
    static jfieldID mayfly;
    static jfieldID flying;
};