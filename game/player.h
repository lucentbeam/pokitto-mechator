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

class Player {
    Controls m_controller;

    Steering m_soldier;
    Steering m_jeep;

    bool m_dismounted = true;

    Rumbler m_shake;

    static Player * s_instance;
public:
    Player();

    void update(float dt);

    void draw(RenderSystem * renderSystem);

    static Vec2f position();
};

#endif // PLAYER_H
