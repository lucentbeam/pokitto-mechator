#ifndef PLAYER_H
#define PLAYER_H

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "game/rendering/camera.h"

#include "game/tilesets.h"
#include "game/physics/steering.h"
#include "game/entities/projectile.h"

#include "game/entities/effects.h"

#include "game/utilities/rumbler.h"
#include "game/utilities/playerstats.h"
#include "game/enums.h"

class Player {
    Steering m_soldier;
    Steering m_jeep;

    Rumbler m_shake;

    PlayerMode m_mode = PlayerMode::Soldier;

    static Player * s_instance;

    uint8_t m_unlock_level = 1;
    bool m_built_vehicles[4] = {false, false, false, false};

public:

    static PlayerStats s_stats;

    static PlayerMode mode() { return s_instance->m_mode; }

    Player();

    void update(float dt);

    void draw();

    static Vec2f position();

    static uint8_t unlockLevel() { return s_instance->m_unlock_level; }
};

#endif // PLAYER_H
