#pragma once

#include <string>


struct Classes {

    static constexpr const char* MINECRAFT = "net/minecraft/client/Minecraft";
    static constexpr const char* PLAYER = "net/minecraft/client/player/LocalPlayer";
    static constexpr const char* ABILITIES = "net/minecraft/world/entity/player/Abilities";

    static std::string sig(const char* className) {
        return std::string("L") + className + ";";
    }
};
