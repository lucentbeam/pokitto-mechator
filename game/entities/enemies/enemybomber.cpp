#include "enemybomber.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"

void EnemyBomber::setup(const Vec2f &pos)
{
    m_pos.set(pos.x(), pos.y());
    status = Waiting;
}

bool EnemyBomber::update(float dt)
{
    static const float velocity = 60.0f;
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::AirTarget});

    int damage;
    static int counter = 0;

    Vec2f delta = Camera::center() - m_pos;

    switch(status) {
    case Waiting:
        if (counter > 0) counter--;
        if (!Camera::inActiveZone(m_pos)) {
            return false;
        }
        if (counter == 0 && delta.length() < 45) {
            status = InSky;
            m_plane_pos.set(m_pos.x() - 70, Camera::center().y() - 20);
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
        counter++;
        if (counter >= 20) {
            counter = 0;
            ProjectileManager::create(m_plane_pos + Vec2f(0, 15), Vec2f(velocity * -0.2f, (rand() % 30) - 15), 4, 1.5f)
                ->setSprite({projectile_grenade[0], projectile_grenade[1]}, 4)
                ->setTargetMask({})
                ->setDamage(0)
                ->setInAir(20, 0)
                ->setExpireCallback([](Projectile*p) {
                ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget});
                    EffectManager::create(p->pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
                });
        }

        damage = m_damage_frames > 0 ? 0 : ProjectileManager::getCollisionDamage(m_pos, 17, bulletMask);
        m_life -= damage;
        if (m_damage_frames > 0) m_damage_frames--;
        if (m_life <= 0) {
            Pickups::spawnDollar(m_plane_pos);

            EffectManager::create(m_plane_pos, {explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
            EffectManager::create(m_plane_pos + Vec2f(-2, -3), {explosion_small[0], explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
            EffectManager::create(m_plane_pos + Vec2f(3, -1), {explosion_small[0], explosion_small[0], explosion_small[0], explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
            EffectManager::create(m_plane_pos + Vec2f(-4, 3), {explosion_small[0], explosion_small[0], explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
            EffectManager::create(m_plane_pos + Vec2f(0, -4), {explosion_small[0], explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
            EffectManager::create(m_plane_pos + Vec2f(-5, 1), {explosion_small[0], explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);

            status = Waiting;
        } else {
            if (damage > 0) {
                m_damage_frames = 6;
                EffectManager::create(m_pos, {hit[0], hit[1], hit[2], hit[3], hit[4]}, 20.0f);
            }
        }
        if ((m_plane_pos.x() - Camera::tl_x()) < -10.0f) {
            status = Waiting;
            counter = 240 + (rand() % 120);
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
