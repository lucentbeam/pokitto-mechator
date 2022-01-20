#include "enemymech.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"

bool EnemyMech::update(float dt, bool check_collisions)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone(m_pos)) return false;
    if (!Camera::inViewingZone(m_pos)) return true;

    float px = Camera::tl_x();
    float py = Camera::tl_y();
    float tx = px + 55;
    float ty = py + 44;

    m_counter++;
    Vec2f dir = Camera::center() - m_pos;
    float len = dir.length();
    if (len > 0) {
        dir = dir / len;
    }
    switch (status) {
    case EnemyMech::Mode::Walking:
        if (len < 10) {
            dir *= -1;
        } else if (len < 20) {
            dir *= 0;
        }
        if (m_counter % asCounts(0.66f) == 0) {
            Vec2f target = Pathfinding::getPath(m_pos, Vec2f(tx, ty), mask) * 6 + Vec2f(3,3);
            dir = target - m_pos;
            float len = dir.length();
            if (len > 0) {
                dir = dir / len;
            }
            m_velocity = dir * 15.0f;
        }
        if (m_counter > asCounts(3.0f)) {
            status = EnemyMech::Mode::Preparing;
            m_velocity = {0, 0};
            m_counter = rand() % 10;
        }
        break;
    case EnemyMech::Mode::Preparing:
        if (m_counter > asCounts(1.0f)) {
            if (Camera::inViewingZone(m_pos)) {
                AudioSystem::play(sfxEnemyShoot);
                ProjectileManager::create(m_pos, dir * 50.0f, 2, 3.0)
                        ->setSprite(BulletSmall)
                        ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
            }
            status = EnemyMech::Mode::Walking;
            m_counter = rand() % asCounts(0.66f);
        }
        break;
    default:
        break;
    }
    Vec2f pos = CollisionManager::resolveMovement(m_pos + Vec2f(0, 2), m_velocity * physicsTimestep, mask);
    m_pos.set(pos.x(), pos.y() - 2);
    int damage = !check_collisions ? 0 : ProjectileManager::getCollisionDamage(pos, 4, bulletMask);
    m_life -= damage;
    if (m_damage_frames > 0) m_damage_frames--;
    if (m_life <= 0) {
        if (m_drops) Pickups::spawnDollar(m_pos);
        EffectManager::createExplosion(pos, 1, 1);
        AudioSystem::play(sfxExplosionSmall);
        return false;
    } else {
        if (damage > 0) {
            m_damage_frames = 10;
            AudioSystem::play(sfxHit1);
            EffectManager::createHit(pos - Vec2f(3.5f, 3.5f));
        }
    }
    return true;
}

void EnemyMech::draw()
{
    auto pos = Camera::worldToScreen(m_pos);
    if (m_damage_frames > 0) {
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[0], enemy_ground[0][2], 10, false);
    } else if (status == EnemyMech::Mode::Preparing) {
        int idx = m_counter % 10 < 5 ? 1 : 3;
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    } else {
        const int increment = 7;
        int idx = m_counter % (increment*4);
        idx = idx < increment ? 0 : idx < increment*2 ? 1 : idx < increment*3 ? 2 : 1;
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    }
}
