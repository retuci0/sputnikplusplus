#pragma once

#include "../Module.h"
#include "../../sdk/Minecraft.h"
#include "../../sdk/Abilities.h"

#include <memory>


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
    std::unique_ptr<Abilities> getAbilities() const {
        auto player = MinecraftClient::getPlayer();
        if (!player || !player->isValid()) return nullptr;
        return player->getAbilities();
    }
};