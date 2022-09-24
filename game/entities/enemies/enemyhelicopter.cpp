#include "enemyhelicopter.h"
#include "enemytank.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "core/audiosystem.h"

bool EnemyHelicopter::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::AirTarget});

    if (m_destroy_out_of_range && !Camera::inActiveZone(m_steering.pos())) return false;
    if (!Camera::inViewingZone(m_steering.pos())) return true;

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
            if (Camera::inViewingZone(m_steering.pos())) {
                AudioSystem::play(sfxEnemyShoot);
                ProjectileManager::create({m_steering.pos().x(), m_steering.pos().y()}, dir * 50.0f, 2, 3.0)
                        ->setSprite(BulletSmall)
                        ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
            }
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
        AudioSystem::play(sfxExplosionBig);
        EffectManager::createExplosion(m_steering.pos(), 10, 10);
        return false;
    } else {
        if (damage > 0) {
            AudioSystem::play(sfxHit1);
            m_damage_frames = 12;
            EffectManager::createHit(m_steering.pos() - Vec2f(3.5f, 3.5f));
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
        if ((m_counter % 3) == 2) RenderSystem::sprite(pos.x() + (m_steering.facing().x() > 0 ? 1 : 0), pos.y(), helicopter_blades[1 + (m_counter % 6)/3], helicopter_blades[0][2], 10, false);
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_helicopter[m_steering.getRotationFrame(m_steering.aim(), 4.0f)], enemy_helicopter[0][2], m_steering.aim().x() > 0);
        if ((m_counter % 3) == 2) RenderSystem::sprite(pos.x() + (m_steering.facing().x() > 0 ? 1 : 0), pos.y(), helicopter_blades[1 + (m_counter % 6)/3], helicopter_blades[0][2]);
    }
}
