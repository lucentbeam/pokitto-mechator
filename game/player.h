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
#include "game/utilities/ticker.h"
#include "game/enums.h"

#include "game/constants.h"

#include "game/weapons.h"
#include "game/variables.h"

const SteeringConfig steering_soldier(soldierSpeed, 1.0,
       (1 << Terrain::Wall) | (1 << Terrain::WaterDeep) | (1 << Terrain::DestrucableWood) | (1 << Terrain::DestructableMetal) | (1 << Terrain::LowWall),
       4, 4);

const SteeringConfig steering_jeep(jeepSpeed, jeepCornering,
       (1 << Terrain::Wall) | (1 << Terrain::WaterDeep) | (1 << Terrain::WaterShallow) | (1 << Terrain::DestrucableWood) | (1 << Terrain::DestructableMetal) | (1 << Terrain::LowWall),
       9, 9, jeepFriction);

const SteeringConfig steering_tank(tankSpeed, tankCornering,
       (1 << Terrain::Wall) | (1 << Terrain::WaterDeep) | (1 << Terrain::WaterShallow) | (1 << Terrain::DestrucableWood) | (1 << Terrain::DestructableMetal) | (1 << Terrain::LowWall),
       14, 14);

const SteeringConfig steering_boat(boatSpeed, boatCornering,
       (1 << Terrain::Wall) | (1 << Terrain::WaterShallow) | (1 << Terrain::None) | (1 << Terrain::Grass) | (1 << Terrain::Mud) | (1 << Terrain::DestrucableWood) | (1 << Terrain::DestructableMetal) | (1 << Terrain::LowWall),
       12, 12, boatFriction);

const SteeringConfig steering_heli(heliSpeed, heliCornering, 0, 14, 14, heliFriction);

struct WeaponHelper {
    int index = 0;
    int count = 0;

    static WeaponHelper getIndexAndMax(int current, int owned);
};

class Player;

class Vehicle {
    GenericTicker m_iframes;

protected:
    Steering m_steering;
    Statistic m_health;
    Rumbler m_shake;

    Vec2f m_aim;


    friend Player;

    Vehicle(int8_t hp, float x, float y, const SteeringConfig * config);

    void flash() { m_iframes.reset(playerIframeLength); }

public:
    bool flashing() const { return !m_iframes.ready(); }

    void updateFlash() {
        m_iframes.update();
    }
};

class Soldier : public Vehicle {
    static Soldier s_instance;

    Vec2f m_aim = Vec2f(0,0);

    bool sprinting = false;
    PlayerMode m_overlaps = SoldierMode;
    uint8_t m_z = 0;

    static constexpr const int s_possible_weapons = Weapon::Gun | Weapon::MachineGun;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Soldier() : Vehicle(8, playerStartTileX*6 + 3, playerStartTileY*6 + 3, &steering_soldier) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static bool overlaps(PlayerMode mode) { return s_instance.m_overlaps == mode; }
    static bool isSprinting() { return s_instance.sprinting; }
    static void setZ(uint8_t z) { s_instance.m_z = z; }

    static void update(float dt);
    static void draw();
    static void drawAir();
};

class Jeep : public Vehicle {
    static Jeep s_instance;

    static constexpr const int s_possible_weapons = Weapon::DualShot | Weapon::MachineGun | Weapon::Grenade;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Jeep() : Vehicle(12, 26*6, 8*6, &steering_jeep) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return GameVariables::hasBlueprintUnlocked(JeepBP); }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static void update(float dt);
    static void draw();
};


class Tank : public Vehicle {
    static Tank s_instance;

    static constexpr const int s_possible_weapons = Weapon::MachineGun | Weapon::Missiles;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Tank() : Vehicle(28, 12*6, 8*6, &steering_tank) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return GameVariables::hasBlueprintUnlocked(TankBP); }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static void update(float dt);
    static void draw();
};

class Boat : public Vehicle {
    static Boat s_instance;

    static constexpr const int s_possible_weapons = Weapon::MachineGun;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Boat() : Vehicle(20, 12*6, 6*6, &steering_boat) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos(); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return GameVariables::hasBlueprintUnlocked(BoatBP); }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static void update(float dt);
    static void draw();
};

class Helicopter : public Vehicle {
    static Helicopter s_instance;

    bool m_inAir = false;

    float m_z = 0.0f;

    static constexpr const int s_possible_weapons = Weapon::MachineGun | Weapon::Missiles;
    static int s_owned_weapons;
    static Weapon::Type s_current_weapon;

    friend Player;
public:
    Helicopter() : Vehicle(16, 12*6, 8*6, &steering_heli) {}

    static Statistic& health() { return s_instance.m_health; }
    static bool damaged() { return s_instance.m_health.value() < s_instance.m_health.max(); }
    static void setPosition(const Vec2f &pos) { s_instance.m_steering.setPos(pos); }
    static Vec2f position() { return s_instance.m_steering.pos() + Vec2f(0, -s_instance.m_z); }
    static bool alive() {return s_instance.m_health.value() > 0; }
    static bool available() { return GameVariables::hasBlueprintUnlocked(HeliBP); }
    static void launch() { s_instance.m_inAir = true; }
    static Rect bounds() { return s_instance.m_steering.rect(); }

    static bool active() { return s_instance.alive() && s_instance.m_inAir && s_instance.m_z >= 20.0f; }

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

    static Vehicle &getInstance(PlayerMode mode);
public:

    static PlayerMode mode() { return s_mode; }


    static bool buildVehicleAt(PlayerMode mode, Vec2f position);

    static bool canGetPickups() { return s_mode != PlayerMode::HelicopterMode; }

    static bool hurting();

    static bool alive(PlayerMode m);
    static bool damaged(PlayerMode m);
    static bool dead();

    static Vec2f position();
    static Rect bounds();

    static Weapon::Type currentWeapon();
    static void cycleWeaponNext();
    static void cycleWeaponPrev();

    static WeaponHelper getCurrentWeaponInfo();

    static bool weaponCooldown(float dt);    

    static void drawReticle(PlayerMode mode, const Vec2f &dir);
    static void drawFlashlight();

    static void updateCounter();

    static void storeData();
    static void loadData();
};

#endif // PLAYER_H
