#include "weapons.h"

#include "game/utilities/mapmanager.h"
#include "game/physics/collisionmanager.h"
#include "core/audiosystem.h"
#include "game/funcs.h"

const WeaponConfig gun_config(5.0f, 3, 0.5f, 100.0f);

const WeaponConfig mgun_config(14.0f, 2, 0.4f, 150.0f);

const WeaponConfig dualshot_config(4.2f, 3, 0.35f, 130.0f);

const WeaponConfig grenade_config(2.8f, 4, 0.5f, 65.0f);

const WeaponConfig missile_config(2.0f, 4, 0.35f, 140.0f);

const WeaponConfig multimissile_config(1.6f, 4, 0.4f, 85.0f, 30.0f);

bool Weapon::checkFire(Projectile * &p, const Button &action, const WeaponConfig &config, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel)
{
    if (action.held()) {
        p = ProjectileManager::create(pos, fac * (config.speed + config.speedvar * (float(rand() % 200)/100.0f - 1.0f)), config.size, config.lifetime);
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
    case Type::MultiMissiles:
        return "Cluster Shot";
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
            AudioSystem::play(sfxPlayerGun);
            p->setSprite(BulletSmall);
            p->setDamage(1);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            if (air) p->setIgnoreWalls();
            delay = gun_config.seconds_per_shot;
        }
        break;
    case Type::MachineGun:
        dir.rotBy((rand() % 16) - 8);
        if (checkFire(p, action, mgun_config, pos, dir, vel)) {
            AudioSystem::play(sfxPlayerGun);
            p->setSprite(BulletSmall);
            p->setDamage(1);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            if (air) p->setIgnoreWalls();
            delay = mgun_config.seconds_per_shot;
        }
        break;
    case Type::DualShot:
        if (checkFire(p, action, dualshot_config, pos + fac.rot90() * 3.0f, fac, vel)) {
            AudioSystem::play(sfxPlayerGun2x);
            p->setSprite(BulletSmall);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            p->setDamage(1);
            if (air) p->setIgnoreWalls();
        }
        if (checkFire(p, action, dualshot_config, pos - fac.rot90() * 3.0f, fac, vel)) {
            p->setSprite(BulletSmall);
            p->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            p->setDamage(1);
            if (air) p->setIgnoreWalls();
            delay = dualshot_config.seconds_per_shot;
        }
        break;
    case Type::Grenade:
        if (checkFire(p, action, grenade_config, pos, fac, vel)) {
            AudioSystem::play(sfxGrenade);
            p->setSprite(GrenadeSprite)
             ->addVelocity(vel * 0.5f)
             ->setTargetMask({EnemyTarget, GroundTarget})
             ->setDamage(0)
             ->setExpireCallback([](Projectile*p) {
                AudioSystem::play(sfxExplosionBig);
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
    case Type::Missiles:
        if (checkFire(p, action, missile_config, pos, fac, vel)) {
            AudioSystem::play(sfxMissile);
            p->setSprite(MissileSprite1)
             ->setTargetMask({EnemyTarget, GroundTarget, AirTarget})
             ->setDamage(0)
             ->setMissile(pos + fac * 5.0f, fac * missile_config.speed)
             ->setFlipped(dir.x() > 0)
             ->setExpireCallback([](Projectile*p) {
                AudioSystem::play(sfxExplosionBig);
                for(int i = -4; i <= 4; i+=4) {
                    for (int j = -4; j <= 4; j+= 4) {
                        Terrain t = CollisionManager::getTerrainAt(p->pos().x()+i, p->pos().y()+j);
                        if (t == Terrain::DestructableMetal || t == Terrain::DestrucableWood) {
                            MapManager::setTileAt(p->pos().x()+i, p->pos().y()+j, 61);
                        }
                    }
                }
                onPlayerMissileExplode();
                ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(6)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
                EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
            });
            if (air) p->setIgnoreWalls();
            delay = missile_config.seconds_per_shot;
        }
        break;
    case Type::MultiMissiles:
        for(int i = 0; i < 6; ++i) {
            dir = fac * 1;
            dir.rotBy((i * 15 + (rand() % 25)) * (i % 2 == 0 ? 1 : -1));
            if (checkFire(p, action, multimissile_config, pos, dir, vel)) {
                if (i == 0) AudioSystem::play(sfxMissile);
                p->setSprite(MissileSprite1)
                 ->setTargetMask({EnemyTarget, GroundTarget, AirTarget})
                 ->setDamage(0)
                 ->setMissile(pos + fac * 5.0f, fac * multimissile_config.speed)
                 ->setFlipped(dir.x() > 0)
                 ->setIgnoreWalls()
                 ->setExpireCallback([](Projectile*p) {
                    for(int i = -4; i <= 4; i+=4) {
                        for (int j = -4; j <= 4; j+= 4) {
                            Terrain t = CollisionManager::getTerrainAt(p->pos().x()+i, p->pos().y()+j);
                            if (t == Terrain::DestructableMetal || t == Terrain::DestrucableWood) {
                                MapManager::setTileAt(p->pos().x()+i, p->pos().y()+j, 61);
                            }
                        }
                    }
                    onPlayerMissileExplode();
                    AudioSystem::play(sfxExplosionBig);
                    ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(4)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
                    EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                });
                if (air) p->setIgnoreWalls();
                delay = multimissile_config.seconds_per_shot;
            } else {
                break;
            }
        }
        break;
    }
    return delay;
}
