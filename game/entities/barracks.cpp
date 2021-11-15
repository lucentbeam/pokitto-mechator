#include "barracks.h"
#include "projectile.h"
#include "game/utilities/helpers.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/effects.h"
#include "game/sprites.h"

#include "game/physics/collisionmanager.h"
#include "game/physics/pathfinding.h"

ObjectPool<Barracks, 6> Barracks::s_barracks;

int8_t Barracks::s_max_life = 27;

void Barracks::config(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    m_spawn = spawn;
    m_left = left * 6;
    m_top = top * 6;
    m_width = width * 6;
    m_height = height * 6;
    m_life = s_max_life;
    m_collision_rect = Rect(m_left, m_top, m_width, m_height);
    m_spawn_timer = 80 + (rand() % 40);
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

        // check if already dead
        if (MapManager::getTileAt(start[i].m_left, start[i].m_top) == 203) {
            s_barracks.deactivate(i);
            continue;
        }

        // check if out of range. if so: deactivate and restore and damaged tiles
        if (!Camera::inActiveZone(start[i].m_spawn)) {
            if (MapManager::getTileAt(start[i].m_left, start[i].m_top) != 203) {
                switch (start[i].stage()) {
                case Stage::DamagedStage:
                    for(int x = start[i].m_left; x < (start[i].m_width + start[i].m_left); x+=6) {
                        for(int y = start[i].m_top; y < (start[i].m_height + start[i].m_top); y+=6) {
                            int t = MapManager::getTileAt(x, y);
                            MapManager::setTileAt(x, y, t - 4);
                        }
                    }
                    break;
                case Stage::HeavyDamagedStage:
                    for(int x = start[i].m_left; x < (start[i].m_width + start[i].m_left); x+=6) {
                        for(int y = start[i].m_top; y < (start[i].m_height + start[i].m_top); y+=6) {
                            int t = MapManager::getTileAt(x, y);
                            MapManager::setTileAt(x, y, t - 8);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            s_barracks.deactivate(i);
            continue;
        }

        // check for damage
        std::vector<Vec2f> hitlocs;
        int damage = ProjectileManager::getCollisionDamage(start[i].m_collision_rect, mask, hitlocs);
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
        } else {
            if (damage > 0) {
                for(auto p : hitlocs) {
                    EffectManager::create(p - Vec2f(3.5f, 3.5f), {hit[0], hit[1], hit[2], hit[3], hit[4]}, 20.0f);
                }
            }
        }

        // decrement counter and check for spawns
        if (start[i].m_spawn_count < 2) {
            start[i].m_spawn_timer--;
            if (start[i].m_spawn_timer <= 0) {
                start[i].m_spawn_timer = 140 + (rand() % 60);
                static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal}); // todo: make this a static for EnemyMech
                if (Pathfinding::canReach(start[i].m_spawn, Camera::center(), mask)) {
                    EnemyMech * m = Enemy::createMech(start[i].m_spawn);
                    ++start[i].m_spawn_count;
                    auto ptr = &start[i].m_spawn_count;
                    m->setDeactivateCallback([=](){
                        --(*ptr);
                    });
                }
            }
        }


        ++i;
    }
}
