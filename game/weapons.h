#ifndef WEAPONS_H
#define WEAPONS_H

#include "core/rendering/camera.h"
#include "core/utilities/vec.h"
#include "core/controls.h"

#include "game/sprites.h"
#include "game/entities/projectile.h"
#include "game/entities/effects.h"

#include "game/enums.h"
#include "game/constants.h"

#include <string>

struct WeaponConfig {
    static constexpr float frames_per_second = 1.0f / physicsTimestep;
    const float shots_per_second;
    const float frames_per_shot;
    const float seconds_per_shot;

    const int size;
    const float lifetime;
    const float speed;

    constexpr WeaponConfig(float sps, int sz, float life, float spd) :
        shots_per_second(sps),
        frames_per_shot(frames_per_second / shots_per_second),
        seconds_per_shot(1.0f / shots_per_second),
        size(sz),
        lifetime(life),
        speed(spd)
    {}
};

class Weapon
{
    static bool checkFire(Projectile * &p, const Button &action, const WeaponConfig &config, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel);
public:

    enum Type {
        // soldier
        Gun = 1,
        MachineGun = 2,

        // jeep
        DualShot = 4,
        Grenade = 8,

        // tank
        Missiles = 16
    };
    static constexpr const Type AllWeapons[] = { Gun, MachineGun, DualShot, Grenade, Missiles };
    static constexpr const int WeaponCount = 5;

    static std::string getName(Type t);

    static Type nextWeapon(Type t);
    static Type prevWeapon(Type t);

    static float checkFireWeapon(const Button &action, Type typ, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel);

};

#endif // WEAPONS_H
