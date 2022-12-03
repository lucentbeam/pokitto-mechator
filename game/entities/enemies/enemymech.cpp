#include "enemymech.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"

bool EnemyMech::update(float dt, bool check_collisions)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    bool shooting = m_counter+1 > asCounts(1.0f);
    bool alive = EnemyAIHelper::updateEntity(m_pos, m_velocity, m_counter, status, m_life, m_damage_frames, mask, bulletMask, check_collisions, m_drops, shooting);
    float p_range = Player::mode() == JeepMode ? 6 : Player::mode() == TankMode ? 8 : 0;
    if (alive && p_range > 0 && Player::moving() && (m_pos - Player::position()).length() < p_range) {
        alive = false;
        if (!(Player::mode() == JeepMode && GameVariables::hasBlueprint(Blueprints::RunoverBP))) ProjectileManager::create(m_pos, {0, 0}, 12, 0.1)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget});
        EffectManager::createExplosionBig(m_pos - Vec2f(6,6));
        AudioSystem::play(sfxExplosionBig);
        if (m_drops) Pickups::spawnDollar(m_pos);
    }
    if (alive) {
        if (shooting) {
            ProjectileManager::create(m_pos, m_velocity * 50.0f, 2, 3.0)
                    ->setSprite(BulletSmall)
                    ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
        }
        if (status == AIMode::Walking) {
            Vec2f mpos = CollisionManager::resolveMovement(m_pos + Vec2f(0, 2), m_velocity * 15.0f * physicsTimestep, mask);
            m_pos.set(mpos.x(), mpos.y() - 2);
        }
    }
    return alive;
}

void EnemyMech::draw()
{
    auto pos = Camera::worldToScreen(m_pos);
    if (m_damage_frames > 0) {
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[0], enemy_ground[0][2], 10, false);
    } else if (status == AIMode::Preparing) {
        int idx = m_counter % 10 < 5 ? 1 : 3;
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    } else {
        const int increment = 7;
        int idx = m_counter % (increment*4);
        idx = idx < increment ? 0 : idx < increment*2 ? 1 : idx < increment*3 ? 2 : 1;
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    }
}
