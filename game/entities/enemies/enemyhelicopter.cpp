#include "enemyhelicopter.h"
#include "enemytank.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"

bool EnemyHelicopter::update(float dt)
{
    static uint16_t mask = Helpers::getMask({});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::AirTarget});

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
    static int shotcount = 0;
    switch (status) {
    case EnemyHelicopter::Mode::Walking:
        shotcount = 0;
        if (len < 20) {
            dir *= 0;
        }
        m_steering.update(dt, m_aim.x(), m_aim.y());
        if (m_counter % 40 == 0) {
            Vec2f alt = {float(rand() % 100) - 50, float(rand() % 100) - 50};
            alt = alt / 50.0f;
            dir = Pathfinding::getPath(Vec2f(m_steering.pos().x(), m_steering.pos().y()), Vec2f(tx, ty), mask) * 6 + Vec2f(3,3) - m_steering.pos();
            float len = dir.length();
            if (len > 0) {
                dir = dir / len;
            }
            m_aim = dir;
        }
        if (m_counter > 180) {
            status = EnemyHelicopter::Mode::Preparing;
            m_aim = {0, 0};
            m_counter = rand() % 10;
        }
        break;
    case EnemyHelicopter::Mode::Preparing:
        m_aim = dir;
        m_steering.update(0, m_aim.x(), m_aim.y());
        if (m_counter > (shotcount == 0 ? 120 : 45)) {
            ProjectileManager::create({m_steering.pos().x(), m_steering.pos().y()}, dir * 50.0f, 2, 3.0)
                    ->setSprite({projectile[0], projectile[1]}, 20.0)
                    ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
            m_counter = rand() % 10;
            shotcount++;
            if (shotcount >= 3) {
                status = EnemyHelicopter::Mode::Walking;
            }
        }
        break;
    default:
        break;
    }
    int damage = ProjectileManager::getCollisionDamage(m_steering.pos(), 10, bulletMask);
    m_life -= damage;
    if (m_damage_frames > 0) m_damage_frames--;
    if (m_life <= 0) {
        Pickups::spawnDollar(m_steering.pos());
        EffectManager::create(m_steering.pos(), {explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
        return false;
    } else {
        if (damage > 0) {
            m_damage_frames = 12;
            EffectManager::create(m_steering.pos() - Vec2f(3.5f, 3.5f), {hit[0], hit[1], hit[2], hit[3], hit[4]}, 20.0f);
        }
    }
    return true;
}

void EnemyHelicopter::draw() const
{
    auto pos = Camera::worldToScreen(m_steering.pos()) - Vec2f(9.5f, 9.5f);
    RenderSystem::drawShadow(pos.x(), pos.y() + 10, enemy_helicopter[m_steering.getRotationFrame(m_steering.aim(), 4.0f)], enemy_helicopter[0][2], m_steering.aim().x() > 0);
}

void EnemyHelicopter::drawAir() const
{
    auto pos = Camera::worldToScreen(m_steering.pos()) - Vec2f(9.5f, 9.5f);
    if (m_damage_frames > 0) {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_helicopter[m_steering.getRotationFrame(m_steering.aim(), 4.0f)], enemy_helicopter[0][2], 10, m_steering.aim().x() > 0);
        if ((m_counter % 3) == 2) RenderSystem::sprite(pos.x() + (m_steering.facing().x() > 0 ? 1 : 0), pos.y(), helicopter_blades[1 + (m_counter % 12)/3], helicopter_blades[0][2], 10, false);
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_helicopter[m_steering.getRotationFrame(m_steering.aim(), 4.0f)], enemy_helicopter[0][2], m_steering.aim().x() > 0);
        if ((m_counter % 3) == 2) RenderSystem::sprite(pos.x() + (m_steering.facing().x() > 0 ? 1 : 0), pos.y(), helicopter_blades[1 + (m_counter % 12)/3], helicopter_blades[0][2]);
    }
}
