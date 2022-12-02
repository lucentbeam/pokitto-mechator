#ifndef WEAPONS_H
#define WEAPONS_H

#include "core/rendering/camera.h"
#include "core/audiosystem.h"
#include "core/utilities/vec.h"
#include "core/controls.h"

#include "game/sprites.h"
#include "game/entities/projectile.h"
#include "game/entities/effects.h"

#include "game/enums.h"
#include "game/constants.h"

#include <string>

struct WeaponConfig {
    const float seconds_per_shot;

    const int size;
    const float lifetime;
    const float speed;
    const float speedvar;
    const SFX sound;

    constexpr WeaponConfig(float sps, int sz, float life, float spd, SFX s) :
        seconds_per_shot(1.0f / sps),
        size(sz),
        lifetime(life),
        speed(spd),
        speedvar(0),
        sound(s)
    {}

    constexpr WeaponConfig(float sps, int sz, float life, float spd, float spdvar, SFX s) :
        seconds_per_shot(1.0f / sps),
        size(sz),
        lifetime(life),
        speed(spd),
        speedvar(spdvar),
        sound(s)
    {}
};

class Weapon
{
    static void makeExplosion(float x, float y, bool break_metal);
    static Projectile * fireWeapon(const WeaponConfig &config, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel, bool air, int dmg, SpriteName s, uint16_t mask, bool playsound = true);
public:

    enum Type {
        // soldier
        Gun = 1,
        Shotgun = 2,
        MachineGun = 4,

        // jeep
        DualShot = 8,
        Grenade = 16,

        // tank
        Missiles = 32,

        // helicopter
        MultiMissiles = 64,

        Spreader = 128,
        Multinade = 256,
        FlameThrower = 512,
    };
    static constexpr const Type AllWeapons[] = { Gun, Shotgun, MachineGun, DualShot, Grenade, Missiles, MultiMissiles, Spreader, Multinade, FlameThrower };
    static constexpr const int WeaponCount = 10;

    static std::string getName(Type t);
    static WeaponConfig getConfig(Type t);

    static Type nextWeapon(Type t);
    static Type prevWeapon(Type t);

    static float checkFireWeapon(const Button &action, Type typ, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel, bool air = false);

};

#endif // WEAPONS_H
