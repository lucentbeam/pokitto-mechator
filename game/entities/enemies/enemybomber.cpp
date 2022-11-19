#include "enemybomber.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"

int EnemyBomber::drop_counter = 0;

void EnemyBomber::setup(const Vec2f &pos)
{
    m_pos.set(pos.x(), pos.y());
    status = Waiting;
    m_damage_frames = 0;
    m_life = 4;
}

bool EnemyBomber::update(float dt)
{
    static const float velocity = 60.0f;
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::AirTarget});

    int damage;

    Vec2f delta = Camera::center() - m_pos;

    switch(status) {
    case Waiting:
        if (!Camera::inActiveZone(m_pos)) {
            return false;
        }
        if ((drop_counter % 60) == 0 && delta.length() < 40) {
            status = InSky;
            m_plane_pos.set(Camera::center().x() - 70, Camera::center().y() - 20);
            m_life = 5;
        }
        break;
    case InSky:
        m_plane_pos.setX(m_plane_pos.x() + velocity * dt);
        if ((m_plane_pos.x() - Camera::tl_x()) > 150.0f) {
            status = FlyBy;
            m_plane_pos.setY(Camera::center().y() - 15);
        }
        break;
    case FlyBy:
        m_plane_pos.setX(m_plane_pos.x() - velocity * dt);
        if ((drop_counter + int(m_pos.x())) % 16 == 0) {
            AudioSystem::play(sfxGrenade);
            ProjectileManager::create(m_plane_pos + Vec2f(0, 15), Vec2f(velocity * -0.2f, (rand() % 30) - 15), 4, 1.5f)
                ->setSprite(GrenadeSprite)
                ->setTargetMask({})
                ->setDamage(0)
                ->setInAir(20, 0)
                ->setExpireCallback([](Projectile*p) {
                ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget});
                    EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                    AudioSystem::play(sfxExplosionBig);
                });
        }

        damage = m_damage_frames > 0 ? 0 : ProjectileManager::getCollisionDamage(m_plane_pos, 17, bulletMask);
        m_life -= damage;
        if (m_damage_frames > 0) m_damage_frames--;
        if (m_life <= 0) {
            Pickups::spawnDollar(m_plane_pos);
            EffectManager::createExplosion(m_plane_pos, 10, 8);
            status = Waiting;
            AudioSystem::play(sfxExplosionBig);
        } else {
            if (damage > 0) {
                m_damage_frames = 6;
                AudioSystem::play(sfxHit1);
                EffectManager::createHit(m_plane_pos);
            }
        }
        if ((m_plane_pos.x() - Camera::tl_x()) < -10.0f) {
            status = Waiting;
        }
        break;
    }

    return true;
}

void EnemyBomber::draw() const
{
    if (status == Waiting || m_life <= 0) return;
    auto pos = Camera::worldToScreen(m_plane_pos) - Vec2f(8.5f, 8.5f);
    RenderSystem::drawShadow(pos.x(), pos.y() + 10, enemy_plane, enemy_plane[2], status == InSky);
}

void EnemyBomber::drawAir() const
{
    if (status != FlyBy || m_life <= 0) return;
    auto pos = Camera::worldToScreen(m_plane_pos) - Vec2f(8.5f, 8.5f);
    if (m_damage_frames > 0) RenderSystem::sprite(pos.x(), pos.y(), enemy_plane, enemy_plane[2], 10, false);
    else RenderSystem::sprite(pos.x(), pos.y(), enemy_plane, enemy_plane[2]);
}
