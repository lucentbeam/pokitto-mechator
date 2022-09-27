#include "enemylasers.h"

#include "core/rendersystem.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"
#include "game/entities/projectile.h"
#include "game/constants.h"
#include "core/audiosystem.h"

constexpr int laserDelay = 170;
constexpr int laserDelayVariation = 40; // delay = laserDelay - (rand() % laserDelayVariation)
constexpr int laserFireLength = 50;
constexpr int laserFireCount = 3;

void EnemyLasers::setup(const Vec2f &pos, bool vert, int sz)
{
    vertical = vert;
    m_pos.set(pos.x(), pos.y());
    size = sz;
    m_counter = rand() % laserDelayVariation;
}

bool EnemyLasers::update(float dt)
{
    if (!Camera::inActiveZone(m_pos)) return false;
    if (!Camera::inViewingZone(m_pos)) return true;

    m_counter++;

    if (m_counter >= laserDelay) {
        int d = m_counter - laserDelay;
        if (d >= laserFireLength * laserFireCount) {
            m_counter = rand() % laserDelayVariation;
        } else if ((d % laserFireLength) == 0) {
            AudioSystem::play(sfxLaser);
            ProjectileManager::create(Vec2f(m_pos.x(), m_pos.y()) + Vec2f(vertical ? 3 : size * 3, vertical ? size * 3 : 3), {0, 0}, vertical ? size * 6 : 6, float(laserFireLength) * 2/3 * physicsTimestep, vertical ? 6 : size * 6)
                    ->setDamage(3)
                    ->setIgnoreWalls()
                    ->clearSprite()
                    ->setTargetMask({PlayerTarget, EnemyTarget, GroundTarget});
        }
    } else if (m_counter == (laserDelay - 40)) {
        AudioSystem::play(sfxLaserCharge);
    }

    return true;
}

void EnemyLasers::draw() const
{
    auto pos = Camera::worldToScreen(m_pos);
    Vec2f pos2 = Vec2f(0.0f, (size - 1) * 6 + 3);
    if (!vertical) pos2 = Vec2f(pos2.y(), pos2.x());
    pos2 += pos;
    const uint8_t (*spr)[20] = vertical ? enemy_laser_ns : enemy_laser_ew;
    RenderSystem::sprite(pos.x(), pos.y(),   spr[1], spr[0][2]);
    RenderSystem::sprite(pos2.x(), pos2.y(), spr[0], spr[0][2]);
    bool charge = m_counter > (laserDelay - 45) && m_counter < (laserDelay - 35);
    if (m_counter < laserDelay && !charge) return;
    Vec2f off1 = Vec2f(3 - (rand() % 2), 3);
    Vec2f off2 = Vec2f(3 - (rand() % 2), 0);
    if (!vertical) {
        off1 = Vec2f(off1.y(), off1.x());
        off2 = Vec2f(off2.y(), off2.x());
    }
    pos += off1;
    pos2 += off2;
    int color = m_counter >= laserDelay ? ((rand() % 3 < 1) ? 19 : 17) : ((rand() % 3 < 1) ? 7 : 6);
    RenderSystem::drawLine(pos.x(), pos.y(), pos2.x(), pos2.y(), color);
}
