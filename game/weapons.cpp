#include "weapons.h"

#include "game/utilities/mapmanager.h"
#include "game/physics/collisionmanager.h"
#include "game/funcs.h"
#include "game/utilities/helpers.h"

const WeaponConfig gun_config(5.0f, 3, 0.5f, 100.0f, sfxPlayerGun);

const WeaponConfig mgun_config(10.0f, 2, 0.3f, 130.0f, 15.0f, sfxPlayerGun);

const WeaponConfig spreader_config(4.0f, 3, 0.5f, 100.0f, sfxPlayerGun2x);

const WeaponConfig dualshot_config(4.4f, 3, 0.35f, 130.0f, sfxPlayerGun2x);

const WeaponConfig grenade_config(2.8f, 4, 0.5f, 65.0f, sfxGrenade);
const WeaponConfig multinade_config(2.4f, 4, 0.5f, 60.0f, sfxGrenade);

const WeaponConfig missile_config(2.0f, 4, 0.35f, 140.0f, sfxMissile);

const WeaponConfig multimissile_config(1.6f, 4, 0.4f, 85.0f, 30.0f, sfxMissile);

const WeaponConfig fthrower_config(20.0f, 4, 0.5f, 50.0f, sfxMissile);

void Weapon::makeExplosion(float x, float y, bool break_metal) {
    for(int i = -4; i <= 4; i+=4) {
        for (int j = -4; j <= 4; j+= 4) {
            Terrain t = CollisionManager::getTerrainAt(x+i, y+j);
            if (t == Terrain::DestrucableWood || (break_metal && t == Terrain::DestructableMetal)) {
                MapManager::setTileAt(x+i, y+j, 61);
            }
        }
    }
}

Projectile * Weapon::fireWeapon(const WeaponConfig &config, const Vec2f &pos, const Vec2f &fac, const Vec2f &vel, bool air, int dmg, SpriteName s, uint16_t mask, bool playsound)
{
    if (playsound) AudioSystem::play(config.sound);
    Projectile * p = ProjectileManager::create(pos, vel * 0.5f + fac * (config.speed + config.speedvar * (float(rand() % 200)/100.0f - 1.0f)), config.size, config.lifetime);
    if (p != nullptr) {
        if (air) p->setIgnoreWalls();
        p->setSprite(s);
        p->setTargetMask(mask);
        p->setDamage(dmg);
    }
    return p;
}

std::string Weapon::getName(Weapon::Type t)
{
    switch(t) {
    case Type::Gun:
        return "Gun";
    case Type::MachineGun:
        return "Machine Gun";
    case Type::DualShot:
        return "Dual Shot";
    case Type::Grenade:
        return "Grenade";
    case Type::Missiles:
        return "Missiles";
    case Type::MultiMissiles:
        return "Cluster Shot";
    case Type::FlameThrower:
        return "Flamethrower";
    case Type::Multinade:
        return "Multinade";
    case Type::Spreader:
        return "Spreader Shot";
    }
    return "Nameless Gun";
}

WeaponConfig Weapon::getConfig(Type t)
{
    switch(t) {
    case Type::Gun:
        return gun_config;
    case Type::MachineGun:
        return mgun_config;
    case Type::DualShot:
        return dualshot_config;
    case Type::Grenade:
        return grenade_config;
    case Type::Missiles:
        return missile_config;
    case Type::MultiMissiles:
        return multimissile_config;
    case Type::Spreader:
        return spreader_config;
    case Type::Multinade:
        return multinade_config;
    case Type::FlameThrower:
        return fthrower_config;
    }
    return gun_config;
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
    if (!action.held()) return 0.0f;
    WeaponConfig cfg = getConfig(typ);
    Vec2f dir = fac * 1;
    uint16_t mask = typ == Type::Grenade ? Helpers::getMask({EnemyTarget, GroundTarget}) : Helpers::getMask({EnemyTarget, GroundTarget, AirTarget});

    static uint8_t counter;
    counter++;
    switch(typ) {
    case Type::Gun:
        dir.rotBy((rand() % 16) - 8);
        fireWeapon(cfg, pos, dir, vel, air, 1, BulletSmall, mask);
        break;
    case Type::MachineGun:
        dir.rotBy((rand() % 40) - 20);
        fireWeapon(cfg, pos, dir, vel, air, 1, BulletSmall, mask);
        break;
    case Type::DualShot:
        fireWeapon(cfg, pos + fac.rot90() * 3.0f, fac, vel, air, 1, BulletSmall, mask, true);
        fireWeapon(cfg, pos - fac.rot90() * 3.0f, fac, vel, air, 1, BulletSmall, mask, false);
        break;
    case Type::Spreader:
        fireWeapon(cfg, pos, dir, vel, air, 1, BulletSmall, mask, true);
        dir.rotBy(20);
        fireWeapon(cfg, pos, dir, vel, air, 1, BulletSmall, mask, false);
        dir.rotBy(-40);
        fireWeapon(cfg, pos, dir, vel, air, 1, BulletSmall, mask, false);
        break;
    case Type::FlameThrower:
        dir.rotBy((rand() % 70) - 35);
        fireWeapon(cfg, pos + dir * 4.0f, dir, vel * 1.5f, air, (rand() % 5) == 0 ? 0 : 1, (rand() % 8 == 0) ? ExplosionBig : ExplosionSmall, mask, (counter % 6) == 0);
        break;
    case Type::Grenade:
        fireWeapon(cfg, pos, fac, vel, air, 0, GrenadeSprite, mask)
                ->setExpireCallback([](Projectile*p) {
                    AudioSystem::play(sfxExplosionBig);
                    makeExplosion(p->pos().x(), p->pos().y(), false);
                    ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget});
                    EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                });
        break;
    case Type::Multinade:
        dir.rotBy(20);
        fireWeapon(cfg, pos, dir, vel, air, 0, GrenadeSprite, mask)
                ->addVelocity(vel * 0.5f)
                ->setExpireCallback([](Projectile*p) {
                    AudioSystem::play(sfxExplosionBig);
                    makeExplosion(p->pos().x(), p->pos().y(), false);
                    ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget});
                    EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                });
        dir.rotBy(-40);
        fireWeapon(cfg, pos, dir, vel, air, 0, GrenadeSprite, mask)
                ->addVelocity(vel * 0.5f)
                ->setExpireCallback([](Projectile*p) {
                    AudioSystem::play(sfxExplosionBig);
                    makeExplosion(p->pos().x(), p->pos().y(), false);
                    ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget});
                    EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                });
        break;
    case Type::Missiles:
        fireWeapon(cfg, pos, fac, vel, air, 0, MissileSprite1, mask)
                ->setMissile(pos + fac * 5.0f, fac * missile_config.speed)
                ->setFlipped(fac.x() > 0)
                ->setExpireCallback([](Projectile*p) {
                    AudioSystem::play(sfxExplosionBig);
                    makeExplosion(p->pos().x(), p->pos().y(), true);
                    onPlayerMissileExplode();
                    ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(6)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
                    EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                });
        break;
    case Type::MultiMissiles:
        for(int i = 0; i < 6; ++i) {
            dir = fac * 1;
            dir.rotBy((i * 15 + (rand() % 25)) * (i % 2 == 0 ? 1 : -1));
            fireWeapon(cfg, pos, dir, vel, true, 0, MissileSprite1, mask, i == 0)
                    ->setMissile(pos + fac * 5.0f, fac * cfg.speed)
                    ->setFlipped(dir.x() > 0)
                    ->setExpireCallback([](Projectile*p) {
                        makeExplosion(p->pos().x(), p->pos().y(), true);
                        onPlayerMissileExplode();
                        AudioSystem::play(sfxExplosionBig);
                        ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(4)->setIgnoreWalls()->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
                        EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                    });
        }
        break;
    }
    return cfg.seconds_per_shot;
}
