#pragma once

#include "../Module.h"
#include "../../sdk/Minecraft.h"
#include "../../sdk/Player.h"
#include "../../sdk/Abilities.h"

#include "../../io.h"


class FlightModule : public Module {
public:
    FlightModule() : Module("vuelo", "sé libre", Category::MOVEMENT, VK_F12) {}

    void onEnable() override {
        if (!mc->getPlayer()) return;

        auto abilities = mc->getPlayer()->getAbilities();
        if (!abilities) return;

        abilities->setMayfly(true);
        //abilities->setFlying(true);
    }

    void onDisable() override {
        if (mc->getPlayer()) {
            auto abilities = mc->getPlayer()->getAbilities();
            if (abilities) {
                abilities->setMayfly(false);
                //abilities->setFlying(false);
            }
        }
    }

    void onTick() override {
        auto player = mc->getPlayer();
        if (player) {
            auto ab = player->getAbilities();
            //if (ab && !ab->isFlying()) {
            //    ab->setFlying(true);
            //}
        }
    }
};