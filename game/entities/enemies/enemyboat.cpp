#include "enemyboat.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"
#include "game/funcs.h"

void EnemyBoat::setup(const Vec2f &pos)
{
    m_origin = {pos.x(), pos.y()};
    m_life = 10;
    m_steering.setPos(pos);
    m_counter = rand() % 30;
}

bool EnemyBoat::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall, Terrain::Mud, Terrain::Grass});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone(m_steering.pos())) {
//        std::cout << "Despawn boat " << m_steering.pos().x() << " " << m_steering.pos().y() << " " << Camera::inActiveZone(m_steering.pos()) << std::endl;
//        Camera::debugShowRegion();
//        std::cout << Camera::center().x() << " " << Camera::center().y() << std::endl;
        return false;
    }
    if (!Camera::inViewingZone(m_steering.pos())) return true;

    float px = Camera::tl_x();
    float py = Camera::tl_y();
    float tx = px + 55;
    float ty = py + 44;

    Vec2f dir = Vec2f(tx, ty) - m_steering.pos();
    float len = dir.length();
    bool flee = false;
    if (len < 30 && len > 6) flee = true;
    if (len > 0) dir = dir / len;

    m_counter++;

    // movement
    if (m_counter % asCounts(0.66f) == 0) {
        dir = Pathfinding::getPath(m_steering.pos(), Vec2f(tx, ty), mask) * 6 + Vec2f(3,3) - m_steering.pos();
        float len = dir.length();
        if (flee) len *= -1;
        if (len > 0) dir = dir / len;
        m_aim = {dir.x(), dir.y()};
    }
    m_steering.update(dt, m_aim.x(), m_aim.y());

    // firing
    if (len < 50 && (m_counter % asCounts(1.5f) == 0) && Camera::inViewingZone(m_steering.pos())) {
        AudioSystem::play(sfxEnemyShoot);
        ProjectileManager::create(m_steering.pos(), dir * 50.0f, 2, 3.0)
                ->setSprite(BulletSmall)
                ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
    }

    // damage & death
    int damage = ProjectileManager::getCollisionDamage(m_steering.pos(), 10, bulletMask);
    m_life -= damage;
    if (m_damage_frames > 0) m_damage_frames--;
    if (m_life <= 0) {
        Pickups::spawnDollar(m_steering.pos());
        EffectManager::createExplosion(m_steering.pos(), 8, 6);
        AudioSystem::play(sfxExplosionSmall);
        return false;
    } else {
        if (damage > 0) {
            m_damage_frames = 12;
            AudioSystem::play(sfxHit1);
            EffectManager::createHit(m_steering.pos() - Vec2f(3.5f, 3.5f));
        }
    }
    return true;
}

void EnemyBoat::draw() const
{
    auto pos = Camera::worldToScreen(m_steering.pos()) - Vec2f(6.0f, 6.0f);
    if (m_damage_frames > 0) {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_boat[m_steering.rotation_frame(4.0f)], enemy_boat[0][2], 10, m_aim.x() > 0);
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_boat[m_steering.rotation_frame(4.0f)], enemy_boat[0][2], m_steering.facing().x() > 0);
    }
}

