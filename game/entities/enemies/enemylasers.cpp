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

void EnemyLasers::setup(const Vec2f &pos, bool vert, int node, int sz)
{
    vertical = vert;
    m_pos.set(vert ? pos.x() : node * 6, vert ? node * 6 : pos.y());
    size = sz;
    m_counter = rand() % laserDelayVariation;
}

bool EnemyLasers::update(float dt)
{
    if (!Camera::inActiveZone(m_pos)) return false;

    m_counter++;

    if (m_counter >= laserDelay) {
        int d = m_counter - laserDelay;
        if (d >= laserFireLength * laserFireCount) {
            m_counter = rand() % laserDelayVariation;
        } else if ((d % laserFireLength) == 0) {
            AudioSystem::play(sfxLaser);
            ProjectileManager::create(m_pos + Vec2f(vertical ? 3 : size * 3, vertical ? size * 3 : 3), {0, 0}, vertical ? size * 6 : 6, float(laserFireLength) * 2/3 * physicsTimestep, vertical ? 6 : size * 6)
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
    if (vertical) {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_laser_ns[1], enemy_laser_ns[0][2]);
        RenderSystem::sprite(pos.x(), pos.y() + (size - 1) * 6 + 3, enemy_laser_ns[0], enemy_laser_ns[0][2]);
        if (m_counter >= laserDelay) {
            RenderSystem::drawLine(pos.x() + 3 - (rand() % 2), pos.y() + 3, pos.x() + 3 - (rand() % 2), pos.y() + (size - 1) * 6 + 3, (rand() % 3 < 1) ? 19 : 17);
        } else if (m_counter > (laserDelay - 45) && m_counter < (laserDelay - 35)) {
            RenderSystem::drawLine(pos.x() + 3 - (rand() % 2), pos.y() + 3, pos.x() + 3 - (rand() % 2), pos.y() + (size - 1) * 6 + 3, (rand() % 3 < 1) ? 7 : 6);
        }
    } else {
        RenderSystem::sprite(pos.x(), pos.y(), enemy_laser_ew[1], enemy_laser_ew[0][2]);
        RenderSystem::sprite(pos.x() + (size - 1) * 6 + 3, pos.y(), enemy_laser_ew[0], enemy_laser_ew[0][2]);
        if (m_counter >= laserDelay) {
            RenderSystem::drawLine(pos.x() + 3, pos.y() + 3 - (rand() % 2), pos.x() + 3 + (size - 1) * 6, pos.y() + 3 - (rand() % 2), (rand() % 3 < 1) ? 19 : 17);
        } else if (m_counter > (laserDelay - 45) && m_counter < (laserDelay - 35)) {
            RenderSystem::drawLine(pos.x() + 3, pos.y() + 3 - (rand() % 2), pos.x() + 3 + (size - 1) * 6, pos.y() + 3 - (rand() % 2), (rand() % 3 < 1) ? 7 : 6);
        }
    }
}
