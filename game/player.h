#ifndef PLAYER_H
#define PLAYER_H

#include "core/rendersystem.h"
#include "core/controls.h"

#include "core/rendering/camera.h"

#include "game/sprites.h"
#include "game/physics/steering.h"
#include "game/entities/projectile.h"

#include "game/entities/effects.h"

#include "game/utilities/rumbler.h"
#include "game/utilities/playerstats.h"
#include "game/enums.h"

#include "game/constants.h"

#include "game/weapons.h"

class Player;

class Vehicle {
protected:
    Steering m_steering;
    Statistic m_health;
    Rumbler m_shake;

    Vec2f m_aim;

    Ticker m_iframes;

    friend Player;

    Vehicle(int8_t hp, float x, float y, float speed, float cornering, std::initializer_list<uint8_t> collisions, float w, float h, float friction = 1.0f);
};

class Soldier : public Vehicle {
    static Soldier s_instance;

    Vec2f m_aim = Vec2f(0,0);

    float sprint_timer = 0.0f;
    static constexpr float sprint_duration = 0.6f;
    static constexpr float sprint_cooldown = 3.0f;
    bool m_overlaps = false;

    static constexpr const int s_possible_weapons = Weapon::Gun | Weapon::MachineGun;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Soldier() : Vehicle(8, playerStartTileX*6, playerStartTileY*6, soldierSpeed, 1.0f, {Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall}, 4, 4) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static bool overlaps() { return s_instance.m_overlaps; }
    static float sprintCooldown() { return s_instance.sprint_timer; }

    static void update(float dt);
    static void draw();
};

class Jeep : public Vehicle {
    static Jeep s_instance;

    static bool s_available;

    static constexpr const int s_possible_weapons = Weapon::DualShot | Weapon::MachineGun | Weapon::Grenade;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Jeep() : Vehicle(12, 26*6, 8*6, jeepSpeed, jeepCornering, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall}, 9, 9, jeepFriction) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static void update(float dt);
    static void draw();
};


class Tank : public Vehicle {
    static Tank s_instance;

    static bool s_available;

    static constexpr const int s_possible_weapons = Weapon::MachineGun | Weapon::Missiles;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Tank() : Vehicle(28, 12*6, 8*6, tankSpeed, tankCornering, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall}, 14, 14) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static void update(float dt);
    static void draw();
};

class Boat : public Vehicle {
    static Boat s_instance;

    static bool s_available;

    static constexpr const int s_possible_weapons = Weapon::MachineGun;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Boat() : Vehicle(20, 12*6, 6*6, boatSpeed, boatCornering, {Terrain::Wall, Terrain::None, Terrain::Grass, Terrain::Mud, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall}, 24, 24, boatFriction) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static void update(float dt);
    static void draw();
};

class Helicopter : public Vehicle {
    static Helicopter s_instance;

    static bool s_available;

    bool m_inAir = false;

    float m_z = 0.0f;

    static constexpr const int s_possible_weapons = Weapon::MachineGun | Weapon::Missiles;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Helicopter() : Vehicle(16, 12*6, 8*6, heliSpeed, heliCornering, {}, 14, 14, heliFriction) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos() + Vec2f(0, -s_instance.m_z); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return s_available; }
    static void launch() { s_instance.m_inAir = true; }
    static Rect bounds() { return s_instance.m_steering.rect(); }

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

    static float s_shot_cooldown;
public:

    static PlayerMode mode() { return s_mode; }

    static bool canGetPickups() { return s_mode != PlayerMode::HelicopterMode; }

    static Vec2f position();
    static Rect bounds();

    static Weapon::Type currentWeapon();
    static void cycleWeaponNext();
    static void cycleWeaponPrev();

    static bool weaponCooldown(float dt);
};

#endif // PLAYER_H
