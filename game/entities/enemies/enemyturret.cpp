#include "enemyturret.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"
#include "game/funcs.h"

bool EnemyTurret::s_enable_all = false;

void EnemyTurret::setup(const Vec2f &pos) {
    m_pos = pos + Vec2f(3, 2);
    m_life = 10;
    if (MapManager::getTileAt(pos.x(), pos.y()) == SpecialTiles::DestroyedTurret) {
        m_life = 0;
    }
    m_disabled = false;
    m_damage_frames = 0;
}

void EnemyTurret::setAllEnabled(bool enabled)
{
    s_enable_all = enabled;
}

void EnemyTurret::setDisabled(bool disable)
{
    m_disabled = disable;
}

bool EnemyTurret::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone(m_pos - Vec2f(3, 2))) {
        if (!s_enable_all || (s_enable_all && m_life <= 0)) {
            m_life = 0;
            return false;
        }
    }
    if ((!s_enable_all && m_disabled) || m_life <= 0) return true;
    if (!Camera::inViewingZone(m_pos)) return true;

    Vec2f dir = Camera::center() - m_pos;
    if (std::abs(dir.x()) > 60 || std::abs(dir.y()) > 50) return true;
    float len = dir.length();
    if (len > 0) dir = dir / len;
    m_counter++;
    m_aim = dir;
    static int shotcount = 0;
    if (m_counter > (shotcount == 0 ? 180 : 40)) {
        if (Camera::inViewingZone(m_pos)) {
            AudioSystem::play(sfxEnemyShootMedium);
            Vec2f f = dir;
            f.rotBy((rand() % 40) - 20);
            ProjectileManager::create(m_pos + dir * 6.0f, f * 33.0f, 4, 3.0)
                    ->setSprite(BulletMedium)
                    ->setTargetMask({PlayerTarget, GroundTarget, AirTarget})
                    ->setDamage(2)
                    ->setIgnoreWalls();
        }
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
        onTurretDestroyed();
        EffectManager::createExplosion(m_pos, 8, 14);
        AudioSystem::play(sfxExplosionBig);
        MapManager::setTileAt(m_pos.x(), m_pos.y(), SpecialTiles::DestroyedTurret);
    } else {
        if (damage > 0) {
            m_damage_frames = 6;
            EffectManager::createHit(m_pos - Vec2f(3.5f, 3.5f));
            AudioSystem::play(sfxHit1);
        }
    }
    m_smoothaim = m_smoothaim * 0.9f + m_aim * 0.1f;
    return true;
}

void EnemyTurret::draw() const
{
    auto pos = Camera::worldToScreen(m_pos) - Vec2f(6.0f, 6.5f);
    int cols[3] = {9, 8, 7};
    if (m_life > 0 && m_damage_frames > 0) {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_turret_base[0], enemy_turret_base[0][2], 10, false);
        cols[0] = 10;
        cols[1] = 10;
        cols[2] = 10;
        Helpers::drawRotatedBox(pos + Vec2f(6.5f, 5.5f), m_smoothaim, 9.0f, cols);
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_turret_base[m_life <= 0 ? 1 : 0], enemy_turret_base[0][2]);
        if (m_life > 0) {
            Helpers::drawRotatedBox(pos + Vec2f(6.5f, 5.5f), m_smoothaim, 9.0f, cols);
        }
    }
}
