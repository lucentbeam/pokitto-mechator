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

const float soldierSpeed = 20.0f;

const float jeepSpeed = 32.0f;
const float jeepGrassSpeedFraction = 0.5f;
const float jeepCornering = 0.1f;
const float jeepFriction = 0.05f;

const float tankSpeed = 25.0f;
const float tankCornering = 0.04f;

const float boatSpeed = 80.0f;
const float boatCornering = 0.1f;
const float boatFriction = 0.01f;

const float heliSpeed = 60.0f;
const float heliCornering = 0.05f;
const float heliFriction = 0.08f;

class Player;

class Vehicle {
protected:
    Steering m_steering;
    Statistic m_health;
    Rumbler m_shake;

    friend Player;

    Vehicle(int8_t hp, float x, float y, float speed, float cornering, std::initializer_list<uint8_t> collisions, float w, float h, float friction = 1.0f);
};

class Soldier : public Vehicle {
    static Soldier s_instance;

    Vec2f m_aim = Vec2f(0,0);
public:
    Soldier() : Vehicle(8, 8*6, 15*6, soldierSpeed, 1.0f, {Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal}, 4, 4) {}
//    Soldier() : Vehicle(8, 36*6, 10*6, 20.0f, 1.0f, {Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal}, 4, 4) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }

    static void update(float dt);
    static void draw();
};

class Jeep : public Vehicle {
    static Jeep s_instance;

    static bool s_available;

public:
    Jeep() : Vehicle(12, 26*6, 8*6, jeepSpeed, jeepCornering, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 9, 9, jeepFriction) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }

    static void update(float dt);
    static void draw();
};


class Tank : public Vehicle {
    static Tank s_instance;

    static bool s_available;
public:
    Tank() : Vehicle(28, 12*6, 8*6, tankSpeed, tankCornering, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 14, 14) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }

    static void update(float dt);
    static void draw();
};

class Boat : public Vehicle {
    static Boat s_instance;

    static bool s_available;
public:
    Boat() : Vehicle(20, 12*6, 6*6, boatSpeed, boatCornering, {Terrain::Wall, Terrain::None, Terrain::Grass, Terrain::Mud, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 24, 24, boatFriction) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }

    static void update(float dt);
    static void draw();
};

class Helicopter : public Vehicle {
    static Helicopter s_instance;

    static bool s_available;

    bool m_inAir = false;

    float m_z = 0.0f;
public:
    Helicopter() : Vehicle(16, 12*6, 8*6, heliSpeed, heliCornering, {}, 14, 14, heliFriction) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos() + Vec2f(0, -s_instance.m_z); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }
    static void launch() { s_instance.m_inAir = true; }

    static void update(float dt);
    static void drawGround();
    static void drawAir();
};

class Player {
    static PlayerMode s_mode;

    friend class Soldier;
    friend class Jeep;
    friend class Tank;
    friend class Boat;
    friend class Helicopter;
public:

    static PlayerMode mode() { return s_mode; }

    static bool canGetPickups() { return s_mode != PlayerMode::HelicopterMode; }

    static Vec2f position();
};

#endif // PLAYER_H
