#include "barracks.h"
#include "projectile.h"
#include "game/utilities/helpers.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/effects.h"

ObjectPool<Barracks, 6> Barracks::s_barracks;

void Barracks::config(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    m_spawn = spawn;
    m_left = left * 6;
    m_top = top * 6;
    m_width = width * 6;
    m_height = height * 6;
    m_life = 30;
    m_collision_rect = Rect(m_left, m_top, m_width, m_height);
    m_spawn_timer = 100 + (rand() % 40);
    m_linked_mechs.clear();
}

void Barracks::create(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    Barracks * b = s_barracks.activateNext();
    if (b == nullptr) {
        return;
    }
    b->config(spawn, left, top, width, height);
}

void Barracks::update(float)
{
    int i = 0;
    Barracks * start = s_barracks.objects();
    static uint16_t mask = Helpers::getMask({Targets::GroundTarget, Targets::EnemyTarget});
    while (i < s_barracks.objectCount()) {
        // check if already dead
        if (MapManager::getTileAt(start[i].m_left, start[i].m_top) == 203) {
            s_barracks.deactivate(i);
            continue;
        }

        // check for damage
        int damage = ProjectileManager::getCollisionDamage(start[i].m_collision_rect, mask);
        Stage current = start[i].stage();
        start[i].m_life -= damage;
        if (start[i].m_life <= 0) {
            for(int x = start[i].m_left; x < (start[i].m_width + start[i].m_left); x+=6) {
                for(int y = start[i].m_top; y < (start[i].m_height + start[i].m_top); y+=6) {
                    MapManager::setTileAt(x, y, 203);
                }
            }
            s_barracks.deactivate(i);
            continue;
        } else if (start[i].stage() != current) {
            for(int x = start[i].m_left; x < (start[i].m_width + start[i].m_left); x+=6) {
                for(int y = start[i].m_top; y < (start[i].m_height + start[i].m_top); y+=6) {
                    int t = MapManager::getTileAt(x, y);
                    MapManager::setTileAt(x, y, t + 4);
                }
            }
        }
        if (damage > 0) {
//            EffectManager::create()
        }

        // decrement counter and check for spawns
//        start[i].m_linked_mechs.erase(std::remove_if(start[i].m_linked_mechs.begin(), start[i].m_linked_mechs.end(), [](EnemyMech *m) { return m->life() <= 0; }));
//        start[i].m_spawn_timer--;
//        if (start[i].m_spawn_timer <= 0 && start[i].m_linked_mechs.size() < 2) {
//            EnemyMech * spawn = Enemy::createAndReturnMech(start[i].m_spawn);
//            if (spawn != nullptr) {
//                start[i].m_linked_mechs.push_back(spawn);
//            }
//            start[i].m_spawn_timer = 100 + (rand() % 40);
//        }

        // check if out of range. if so: deactivate and restore and damaged tiles

        ++i;
    }
}
