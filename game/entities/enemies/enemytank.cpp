#include "enemytank.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"

bool EnemyTank::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone(m_steering.pos())) {
        return false;
    }

    float px = Camera::tl_x();
    float py = Camera::tl_y();
    float tx = px + 55;
    float ty = py + 44;

    Vec2f dir = Vec2f(tx, ty) - m_steering.pos();
    float len = dir.length();
    if (len > 0) {
        dir = dir / len;
    }
    m_counter++;
    switch (status) {
    case EnemyTank::Mode::Walking:
        if (len < 20) {
            dir *= 0;
        }
        m_steering.update(dt, m_aim.x(), m_aim.y());
        if (m_counter % 40 == 0) {
            Vec2f alt = {float(rand() % 100) - 50, float(rand() % 100) - 50};
            alt = alt / 50.0f;
            Vec2f target = Pathfinding::getPath(Vec2f(m_steering.pos().x(), m_steering.pos().y()), Vec2f(tx, ty), mask) * 6 + Vec2f(3,3);
            dir = {target.x() - m_steering.pos().x(), target.y() - m_steering.pos().y()};
            float len = dir.length();
            if (len > 0) {
                dir = dir / len;
            }
            m_aim = dir;
        }
        if (m_counter > 180) {
            status = EnemyTank::Mode::Preparing;
            m_aim = {0, 0};
            m_counter = rand() % 10;
        }
        break;
    case EnemyTank::Mode::Preparing:
        m_aim = dir;
        if (m_counter > 60) {
            ProjectileManager::create({m_steering.pos().x(), m_steering.pos().y()}, dir * 50.0f, 2, 3.0)
                    ->setSprite({projectile[0], projectile[1]}, 20.0)
                    ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
            status = EnemyTank::Mode::Walking;
            m_counter = rand() % 40;
        }
        break;
    default:
        break;
    }
    int damage = ProjectileManager::getCollisionDamage(m_steering.pos(), 4, bulletMask);
    m_life -= damage;
    if (m_life <= 0) {
        Pickups::spawnDollar(m_steering.pos());
        EffectManager::create(m_steering.pos(), {explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
        return false;
    } else {
        if (damage > 0) {
            m_damage_frames = 10;
            EffectManager::create(m_steering.pos() - Vec2f(3.5f, 3.5f), {hit[0], hit[1], hit[2], hit[3], hit[4]}, 20.0f);
        }
    }
    if (m_damage_frames > 0) {
        --m_damage_frames;
    }
    return true;
}

void EnemyTank::draw() const
{
    auto pos = Camera::worldToScreen(m_steering.pos()) - Vec2f(6.0f, 6.5f);
    RenderSystem::sprite(pos.x(), pos.y(), enemy_tank[m_steering.rotation_frame(4.0f)], enemy_tank[0][2], m_steering.facing().x() > 0);
    RenderSystem::sprite(pos.x(), pos.y(), enemy_tank_cannon[Steering::getRotationFrame(m_aim, 4.0f)], enemy_tank_cannon[0][2], m_aim.x() > 0);
}
