#pragma once

#include "../Module.h"
#include "../../sdk/Minecraft.h"
#include "../../sdk/Player.h"
#include "../../sdk/Abilities.h"


class FlightModule : public Module {
public:
    FlightModule() : Module("vuelo", "sé libre", Category::MOVEMENT, VK_F12) {}

    void onEnable() override {
        if (auto ab = getAbilities()) {
            ab->setMayfly(true);
            ab->setFlying(true);
        }
    }

    void onDisable() override {
        if (auto ab = getAbilities()) {
            ab->setMayfly(false);
            ab->setFlying(false);
        }
    }

    void onTick() override {
        if (auto ab = getAbilities()) {
            if (!ab->isFlying()) {
                ab->setFlying(true);
            }
        }
    }

private:
    Abilities* getAbilities() const {
        auto mc = MinecraftClient::getInstance();
        if (!mc) return nullptr;
        auto player = mc->getPlayer();
        if (!player) return nullptr;
        auto ab = player->getAbilities();
        return ab ? ab.release() : nullptr;
    }
};