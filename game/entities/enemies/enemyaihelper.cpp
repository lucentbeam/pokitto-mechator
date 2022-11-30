#include "enemyaihelper.h"

#include "core/audiosystem.h"
#include "core/rendering/camera.h"
#include "game/physics/collisionmanager.h"
#include "game/physics/pathfinding.h"
#include "game/constants.h"
#include "game/entities/effects.h"
#include "game/entities/projectile.h"
#include "game/entities/pickups.h"

bool canHitTarget(Vec2f from) {
    Vec2f dir = Player::position() - from;
    float l = dir.length();
    if (l > 0) {
        dir = dir / l;
    }
    int i = 3;
    from += dir * 3.0f;
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::DestrucableWood, Terrain::DestructableMetal});
    while (i < l) {
        if (CollisionManager::collides(from, mask)) {
            return false;
        }
        i += 3;
        from += dir * 3.0f;
    }
    return true;
}

bool EnemyAIHelper::updateEntity(Vec2f pos, Vec2f &aim, uint16_t &action_counter, AIMode &mode, int8_t &life, uint8_t &damage_frames, uint16_t walkmask, uint16_t bulletmask, bool checkcollisions, bool drops, bool &shooting)
{
    if (!Camera::inActiveZone(pos)) {
        shooting = false;
        return false;
    }
    if (!Camera::inViewingZone(pos)) {
        shooting = false;
        return true;
    }

    float px = Camera::tl_x();
    float py = Camera::tl_y();
    float tx = px + 55;
    float ty = py + 44;

    action_counter++;
    Vec2f dir = Vec2f(tx, ty) - pos;
    float len = dir.length();
    if (len > 0) {
        dir = dir / len;
    }
    switch (mode) {
    case AIMode::Walking:
        if (len < 10) {
            dir *= -1;
        } else if (len < 20) {
            dir *= 0;
        }
        if (action_counter % asCounts(0.66f) == 0) {
            aim = Pathfinding::getPath(pos, Vec2f(tx, ty), walkmask) * 6 + Vec2f(3,3) - pos;
            float len = aim.length();
            if (len > 0) {
                aim = aim / len;
            }
        }
        if (action_counter > asCounts(3.0f) && canHitTarget(pos)) {
            mode = AIMode::Preparing;
            aim = {0, 0};
            action_counter = rand() % 10;
        }
        shooting = false;
        break;
    case AIMode::Preparing:
        aim = dir;
        if (shooting) {
            if (Camera::inViewingZone(pos)) {
                AudioSystem::play(sfxEnemyShoot);
            } else {
                shooting = false;
            }
            mode = AIMode::Walking;
            action_counter = rand() % asCounts(0.66f);
        }
        break;
    }
    int damage = !checkcollisions ? 0 : ProjectileManager::getCollisionDamage(pos, 4, bulletmask);
    life -= damage;
    if (damage_frames > 0) damage_frames--;
    if (life <= 0) {
        if (drops) Pickups::spawnDollar(pos);
        EffectManager::createExplosion(pos, 1, 1);
        AudioSystem::play(sfxExplosionSmall);
        return false;
    } else if (damage > 0) {
        damage_frames = 10;
        AudioSystem::play(sfxHit1);
        EffectManager::createHit(pos - Vec2f(3.5f, 3.5f));
    }
    return true;
}
