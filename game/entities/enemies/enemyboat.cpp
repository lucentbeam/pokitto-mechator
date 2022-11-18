#include "enemyboat.h"

#include "game/entities/enemies/enemyaihelper.h"
#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"
#include "game/funcs.h"

void EnemyBoat::setup(const Vec2f &pos)
{
    m_origin = {pos.x(), pos.y()};
    m_life = 4;
    m_steering.setPos(pos);
    m_counter = rand() % 30;
    m_damage_frames = 0;
}

bool EnemyBoat::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall, Terrain::Mud, Terrain::Grass});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    bool shooting = false;
    AIMode status = AIMode::Walking;
    bool alive = EnemyAIHelper::updateEntity(m_steering.pos(), m_aim, m_counter, status, m_life, m_damage_frames, mask, bulletMask, true, true, shooting);
    if (alive) {
        Vec2f dir = Camera::center() - m_steering.pos();
        float len = dir.length();
        if (len > 0) dir = dir / len;
        if (len < 30 && len > 6) m_aim *= -1;
        if (m_counter % asCounts(1.5f) == 0 && Camera::inViewingZone(m_steering.pos()) && (len < 50)) {
            m_aim = dir;
            ProjectileManager::create(m_steering.pos(), m_aim * 50.0f, 2, 3.0)
                    ->setSprite(BulletSmall)
                    ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
        }
        m_steering.update(dt, m_aim.x(), m_aim.y());
    }
    return alive;
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

