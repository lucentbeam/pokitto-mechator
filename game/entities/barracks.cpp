#include "barracks.h"
#include "projectile.h"
#include "game/utilities/helpers.h"
#include "game/utilities/mapmanager.h"

ObjectPool<Barracks, 6> Barracks::s_barracks;

void Barracks::config(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    m_spawn = spawn;
    m_left = left * 6;
    m_top = top * 6;
    m_width = width * 6;
    m_height = height * 6;
    m_collision_rect = Rect(m_left, m_top, m_width, m_height);
}

void Barracks::create(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    Barracks * b = s_barracks.activateNext();
    if (b == nullptr) {
        return;
    }
    b->config(spawn, left, top, width, height);
}

void Barracks::update(float dt)
{
    int i = 0;
    Barracks * start = s_barracks.objects();
    static uint16_t mask = Helpers::getMask({Targets::GroundTarget, Targets::EnemyTarget});
    while (i < s_barracks.objectCount()) {
        if (ProjectileManager::getCollisionDamage(start[i].m_collision_rect, mask)) {
            for(int x = start[i].m_left; x < (start[i].m_width + start[i].m_left); x+=6) {
                for(int y = start[i].m_top; y < (start[i].m_height + start[i].m_top); y+=6) {
                    MapManager::setTileAt(x, y, 203);
                }
            }
            s_barracks.deactivate(i);
        } else {
            i++;
        }
    }
    // check spawn times
    // check spawn counts
    // check bullets
    // check out of range:
    // - restore tiles
}
