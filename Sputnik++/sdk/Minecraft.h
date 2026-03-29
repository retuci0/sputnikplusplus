#pragma once

#include "../java.h"
#include "Player.h"
#include <memory>


// objeto estático puro: busca el instance y el player cada llamada
class MinecraftClient {
public:
    MinecraftClient() = delete;

    static bool init();

    static std::unique_ptr<Player> getPlayer();

private:
    static jclass clazz;
    static jmethodID getInstanceMethod;
    static jfieldID playerField;
};