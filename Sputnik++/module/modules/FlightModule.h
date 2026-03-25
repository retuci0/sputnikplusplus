#pragma once

#include "../Module.h"
#include "../../sdk/Minecraft.h"
#include "../../sdk/Player.h"
#include "../../sdk/Abilities.h"


class FlightModule : public Module {
public:
    FlightModule() : Module("vuelo", "sé libre", Category::MOVEMENT, VK_F12) {}

    void onEnable() override {
        auto mc = MinecraftClient::getInstance();
        auto player = mc->getPlayer();
        if (player) {
            auto ab = player->getAbilities();
            if (ab) {
                ab->setMayfly(true);
                ab->setFlying(true);
            }
        }
    }

    void onDisable() override {
        auto mc = MinecraftClient::getInstance();
        auto player = mc->getPlayer();
        if (player) {
            auto ab = player->getAbilities();
            if (ab) {
                ab->setMayfly(false);
                ab->setFlying(false);
            }
        }
    }

    void onTick() override {
        if (!isEnabled()) return;
        auto mc = MinecraftClient::getInstance();
        auto player = mc->getPlayer();
        if (player) {
            auto ab = player->getAbilities();
            if (ab && !ab->isFlying()) {
                ab->setFlying(true);
            }
        }
    }
};