#include "enemytank.h"

#include "game/entities/projectile.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"
#include "game/funcs.h"

void EnemyTank::setup(const Vec2f &pos)
{
    m_origin = {pos.x(), pos.y()};
    m_life = 10;
    m_steering.setPos(pos);
    m_counter = rand() % 30;
    status = Mode::Walking;
    m_drops_cash = true;
    m_missiles = false;
    m_deactivate = [](){};
    m_damage_frames = 0;
}

bool EnemyTank::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone(m_steering.pos())) return false;
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
    case EnemyTank::Mode::Walking:
        shotcount = 0;
        if (len < 20) {
            dir *= 0;
        }
        m_steering.update(dt, m_aim.x(), m_aim.y());
        if (m_counter % asCounts(0.66f) == 0) {
            dir = Pathfinding::getPath(m_steering.pos(), Vec2f(tx, ty), mask) * 6 + Vec2f(3,3) - m_steering.pos();
            float len = dir.length();
            if (len > 0) {
                dir = dir / len;
            }
            m_aim = {dir.x(), dir.y()};
        }
        if (m_counter > asCounts(3.0f)) {
            status = EnemyTank::Mode::Preparing;
            m_aim = {0, 0};
            m_counter = rand() % 10;
        }
        break;
    case EnemyTank::Mode::Preparing:
        m_aim = {dir.x(), dir.y()};
        if (m_counter > (shotcount == 0 ? asCounts(1.25f) : asCounts(0.5f))) {
            if (Camera::inViewingZone(m_steering.pos())) {
                AudioSystem::play(sfxEnemyShoot);
                if (true) {
                    Projectile * p = ProjectileManager::create(m_steering.pos() + dir * 6.0f, dir * 50.0f, 2, 3.0)
                     ->setSprite(MissileSprite1)
                     ->setTargetMask({PlayerTarget, GroundTarget, AirTarget})
                     ->setDamage(0)
                     ->setMissile(m_steering.pos() + dir * 5.0f, dir * 90.0f)
                     ->setFlipped(dir.x() > 0);
                    if (m_missiles) {
                         p->setExpireCallback([](Projectile*p) {
                                for(int i = -4; i <= 4; i+=4) {
                                    for (int j = -4; j <= 4; j+= 4) {
                                        Terrain t = CollisionManager::getTerrainAt(p->pos().x()+i, p->pos().y()+j);
                                        if (t == Terrain::DestructableMetal || t == Terrain::DestrucableWood) {
                                            MapManager::setTileAt(p->pos().x()+i, p->pos().y()+j, 61);
                                        }
                                    }
                            }
                            ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
                            EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                            AudioSystem::play(sfxExplosionBig);
                            onEnemyMissileExplode();
                        });
                    } else {
                        p->setExpireCallback([](Projectile*p) {
                            ProjectileManager::create(p->pos(), {0, 0}, 12, 0.1)->setDamage(3)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
                            EffectManager::createExplosionBig(p->pos() - Vec2f(6,6));
                            AudioSystem::play(sfxExplosionBig);
                            onEnemyMissileExplode();
                        });
                    }
                } else {
                    ProjectileManager::create(m_steering.pos() + dir * 6.0f, dir * 50.0f, 2, 3.0)
                            ->setSprite(BulletSmall)
                            ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
                }
            }
            m_counter = rand() % 10;
            shotcount++;
            if (shotcount >= 3) {
                status = EnemyTank::Mode::Walking;
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
        if (m_drops_cash) Pickups::spawnDollar(m_steering.pos());
        m_deactivate();
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
