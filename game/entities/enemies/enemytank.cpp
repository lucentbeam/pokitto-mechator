#include "enemytank.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"
#include "game/funcs.h"

void EnemyTank::setup(const Vec2f &pos)
{
    m_origin = {pos.x(), pos.y()};
    m_life = 7;
    m_steering.setPos(pos);
    m_counter = rand() % 30;
    status = AIMode::Walking;
    m_drops_cash = true;
    m_damage_frames = 0;
}

bool EnemyTank::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    static int shotcount = 0;
    bool shooting = (m_counter + 1) > (shotcount == 0 ? asCounts(1.25f) : asCounts(0.5f));
    bool alive = EnemyAIHelper::updateEntity(m_steering.pos(), m_aim, m_counter, status, m_life, m_damage_frames, mask, bulletMask, true, m_drops_cash, shooting);
    if (alive && status == AIMode::Walking) {
        if (shooting) {
            Projectile * p = ProjectileManager::create(m_steering.pos() + m_aim * 6.0f, m_aim * 50.0f, 2, 3.0)
             ->setSprite(MissileSprite1)
             ->setTargetMask({PlayerTarget, GroundTarget, AirTarget})
             ->setDamage(0)
             ->setMissile(m_steering.pos() + m_aim * 5.0f, m_aim * 90.0f)
             ->setFlipped(m_aim.x() > 0)
             ->setExpireCallback([](Projectile*p) {
                ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
                EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                AudioSystem::play(sfxExplosionBig);
                onEnemyMissileExplode();
             });

            m_counter = rand() % 10;
            shotcount++;
            if (shotcount < 3) status = AIMode::Preparing;
        }
        if (status == AIMode::Walking) {
            m_steering.update(dt, m_aim.x(), m_aim.y());
        }
    } else if (!alive) {
       Pickups::spawnDollar(m_steering.pos() + Vec2i(5, 5));
    }
    return alive;
}

void EnemyTank::draw() const
{
    auto pos = Camera::worldToScreen(m_steering.pos()) - Vec2f(6.0f, 6.5f);
    int cols[3] = {9, 8, 7};
    if (m_damage_frames > 0) {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_tank[m_steering.rotation_frame(4.0f)], enemy_tank[0][2], 10, m_aim.x() > 0);
        cols[0] = 10;
        cols[1] = 10;
        cols[2] = 10;
        Helpers::drawRotatedBox(pos + Vec2f(6.5f, 6.5f), m_aim, 7.0f, cols);
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_tank[m_steering.rotation_frame(4.0f)], enemy_tank[0][2], m_steering.facing().x() > 0);
        Helpers::drawRotatedBox(pos + Vec2f(6.5f, 6.5f), m_aim, 7.0f, cols);
    }
}
