#include "weapons.h"

#include "game/utilities/mapmanager.h"
#include "game/physics/collisionmanager.h"

WeaponConfig gun_config(3.0f, 3, 0.5f, 100.0f);

WeaponConfig mgun_config(8.0f, 2, 0.35f, 150.0f);

WeaponConfig dualshot_config(2.2f, 3, 0.35f, 150.0f);

WeaponConfig grenade_config(1.8f, 4, 0.5f, 50.0f);

bool Weapon::checkFire(Projectile * &p, const Button &action, const WeaponConfig &config, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel)
{
    if (action.held()) {
        p = ProjectileManager::create(pos, fac * config.speed, config.size, config.lifetime);
        return true;
    }
    return false;
}

std::string Weapon::getName(Weapon::Type t)
{
    switch(t) {
    case Type::Gun:
        return "Gun";
        break;
    case Type::MachineGun:
        return "Machine Gun";
        break;
    case Type::DualShot:
        return "Dual Shot";
        break;
    case Type::Grenade:
        return "Grenade";
        break;
    case Type::Missiles:
        return "Missiles";
        break;
    }
    return "Nameless Gun";
}

Weapon::Type Weapon::nextWeapon(Weapon::Type t)
{
    bool next = false;
    for(auto tp : AllWeapons) {
        if (next) return tp;
        if (t == tp) next = true;
    }
    return AllWeapons[0];
}

Weapon::Type Weapon::prevWeapon(Weapon::Type t)
{
    bool next = false;
    for(int i = WeaponCount - 1; i >= 0; --i) {
        if (next) return AllWeapons[i];
        if (AllWeapons[i] == t) next = true;
    }
    return AllWeapons[WeaponCount-1];
}

float Weapon::checkFireWeapon(const Button &action, Weapon::Type typ, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel, bool air)
{
    float delay = 0.0f;
    Projectile * p = nullptr;
    Vec2f dir = fac * 1;
    switch(typ) {
    case Type::Gun:
        if (checkFire(p, action, gun_config, pos, fac, vel)) {
            p->setSprite(projectile[0], 1, 20);
            p->setDamage(1);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            if (air) p->setIgnoreWalls();
            delay = gun_config.seconds_per_shot;
        }
        break;
    case Type::MachineGun:
        dir.rotBy((rand() % 30) - 15);
        if (checkFire(p, action, mgun_config, pos, dir, vel)) {
            p->setSprite(projectile[0], 1, 20);
            p->setDamage(1);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            if (air) p->setIgnoreWalls();
            delay = mgun_config.seconds_per_shot;
        }
        break;
    case Type::DualShot:
        if (checkFire(p, action, dualshot_config, pos + fac.rot90() * 3.0f, fac, vel)) {
            p->setSprite(projectile[0], 1, 20);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            p->setDamage(1);
            if (air) p->setIgnoreWalls();
        }
        if (checkFire(p, action, dualshot_config, pos - fac.rot90() * 3.0f, fac, vel)) {
            p->setSprite(projectile[0], 1, 20);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            p->setDamage(1);
            if (air) p->setIgnoreWalls();
            delay = dualshot_config.seconds_per_shot;
        }
        break;
    case Type::Grenade:
        if (checkFire(p, action, grenade_config, pos, fac, vel)) {
            p->setSprite(projectile_grenade[0], 2, 4)
             ->addVelocity(vel)
             ->setTargetMask({EnemyTarget, GroundTarget})
             ->setDamage(0)
             ->setExpireCallback([](Projectile*p) {
                for(int i = -4; i <= 4; i+=4) {
                    for (int j = -4; j <= 4; j+= 4) {
                        Terrain t = CollisionManager::getTerrainAt(p->pos().x()+i, p->pos().y()+j);
                        if (t == Terrain::DestrucableWood) {
                            MapManager::setTileAt(p->pos().x()+i, p->pos().y()+j, 61);
                        }
                    }
                }
                ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget});
                EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
            });
            if (air) p->setIgnoreWalls();
            delay = grenade_config.seconds_per_shot;
        }
        break;
    }
    return delay;
}
