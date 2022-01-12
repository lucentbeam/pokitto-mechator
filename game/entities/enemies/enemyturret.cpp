#include "enemyturret.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"

void EnemyTurret::setup(const Vec2f &pos) {
    m_pos = pos;
    m_life = 10;
    m_on_deactivate = std::function<void()>();
    if (MapManager::getTileAt(pos.x(), pos.y()) == SpecialTiles::DestroyedTurret) {
        m_life = 0;
    }
}

bool EnemyTurret::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone(m_pos)) {
        return false;
    }

    if (m_life <= 0) return true;

    Vec2f dir = Camera::center() - m_pos;
    if (std::abs(dir.x()) > 60 || std::abs(dir.y()) > 50) return true;
    float len = dir.length();
    if (len > 0) dir = dir / len;
    m_counter++;
    m_aim = dir;
    static int shotcount = 0;
    if (m_counter > (shotcount == 0 ? 180 : 40)) {
        Vec2f f = dir;
        f.rotBy((rand() % 40) - 20);
        ProjectileManager::create(m_pos + dir * 6.0f, f * 33.0f, 4, 3.0)
                ->setSprite({projectile_med[0], projectile_med[1]}, 20.0)
                ->setTargetMask({PlayerTarget, GroundTarget, AirTarget})
                ->setIgnoreWalls();
        m_counter = rand() % 10;
        shotcount++;
        if (shotcount >= 2) {
            shotcount = 0;
        }
    }
    int damage = ProjectileManager::getCollisionDamage(m_pos, 10, bulletMask);
    m_life -= damage;
    if (m_damage_frames > 0) m_damage_frames--;
    if (m_life <= 0) {
        EffectManager::createExplosion(m_pos, 8, 14);
        MapManager::setTileAt(m_pos.x(), m_pos.y(), SpecialTiles::DestroyedTurret);
    } else {
        if (damage > 0) {
            m_damage_frames = 6;
            EffectManager::create(m_pos - Vec2f(3.5f, 3.5f), {hit[0], hit[1], hit[2], hit[3], hit[4]}, 20.0f);
        }
    }
    m_smoothaim = m_smoothaim * 0.9f + m_aim * 0.1f;
    return true;
}

void EnemyTurret::draw() const
{
    auto pos = Camera::worldToScreen(m_pos) - Vec2f(6.0f, 6.5f);
    if (m_damage_frames > 0) {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_turret_base[0], enemy_turret_base[0][2], 10, false);
        RenderSystem::sprite(pos.x(), pos.y(), enemy_tank_cannon[Steering::getRotationFrame(m_smoothaim, 8.0f)], enemy_tank_cannon[0][2], 10, m_smoothaim.x() > 0);
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_turret_base[m_life <= 0 ? 1 : 0], enemy_turret_base[0][2]);
        if (m_life > 0) RenderSystem::sprite(pos.x(), pos.y(), enemy_tank_cannon[Steering::getRotationFrame(m_smoothaim, 8.0f)], enemy_tank_cannon[0][2], m_smoothaim.x() > 0);
    }
}
