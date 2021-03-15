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

class Player;

class Vehicle {
    Steering steering;
    Statistic health;
    Rumbler shake;

    friend Player;

public:
    Vehicle(int8_t hp, float x, float y, float speed, float cornering, std::initializer_list<uint8_t> collisions, float w, float h, float friction = 1.0f);
};

class Player {
    static Vehicle s_soldier, s_jeep;

    static PlayerMode s_mode;

    static bool s_vehicles_available[4];

public:

    static PlayerMode mode() { return s_mode; }

    static Statistic& soldierHealth() { return s_soldier.health; }
    static Statistic& jeepHealth() { return s_jeep.health; }
//    static Statistic& tankHealth() { return s_soldier.health; }
//    static Statistic& boatHealth() { return s_soldier.health; }
//    static Statistic& helicopterHealth() { return s_soldier.health; }

    static bool alive(PlayerMode vehicle);
    static bool damaged(PlayerMode vehicle);
    static bool available(PlayerMode vehicle);

    static void setPosition(PlayerMode vehicle, const Vec2f &pos);

    static void update(float dt);

    static void draw();

    static Vec2f position();
};

#endif // PLAYER_H
