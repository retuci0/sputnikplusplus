#pragma once

#include "../Module.h"
#include "../../sdk/Minecraft.h"
#include "../../sdk/Abilities.h"

#include "../util/KeyUtil.h"

#ifdef _WIN32
#include <windows.h>
#include <WinUser.h>
#endif

#include <memory>


class FlightModule : public Module {
public:
    FlightModule() : Module("vuelo", "sé libre", Category::MOVEMENT, KEY_F12) {}

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