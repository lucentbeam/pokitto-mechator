#include "barracks.h"
#include "projectile.h"
#include "game/utilities/helpers.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/effects.h"
#include "game/sprites.h"

#include "game/physics/collisionmanager.h"
#include "game/physics/pathfinding.h"

#include "game/maps/worldmutables.h"

#include "game/constants.h"

SimplePool<Barracks, 6> Barracks::s_barracks;

int8_t Barracks::s_max_life = 27;


#include <iostream>
void Barracks::config(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    std::cout << "spawning barracks at " << spawn.x() << " " << spawn.y() << std::endl;
    m_spawn = spawn;
    m_left = left * 6;
    m_top = top * 6;
    m_width = width * 6;
    m_height = height * 6;
    m_life = s_max_life;
    m_collision_rect = Rect(m_left, m_top, m_width, m_height);
    m_spawn_timer = 80 + (rand() % 40);
    m_checks_pathfinding = true;
    m_destroy_out_of_range = true;
    m_spawns_tanks = false;

    // If this stuff ever changes, double-check the implementation in TiledMapReader!!!
    int idx = int(left)  + int(top) * 255;
    for(int i = 0; i < sizeof(barracks_indices); ++i) {
        if (idx == barracks_indices[i]) {
            m_barracks_index = i;
            return;
        }
    }
}

void Barracks::disablePathfindingChecks()
{
    m_checks_pathfinding = false;
}

void Barracks::disableDestroyOutOfRange()
{
    m_destroy_out_of_range = false;
}

void Barracks::setSpawnsTanks()
{
    m_spawns_tanks = true;
}

void Barracks::create(const Vec2i &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height)
{
    if (getBarracksAt({left + width/2, top + height/2}) != nullptr) return;
    if (MapManager::getTileAt(left * 6 + 3, top * 6 + 3) == 203) return;
    Barracks * b = s_barracks.activateNext();
    if (b == nullptr) {
        return;
    }
    b->config(spawn, left, top, width, height);
}

void Barracks::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Targets::GroundTarget, Targets::EnemyTarget});
    s_barracks.iterate([&](Barracks * b) {

        b->m_flash.update();

        // check if out of range. if so: deactivate and restore and damaged tiles
        if (b->m_destroy_out_of_range && !Camera::inActiveZone(b->m_spawn)) {
            if (MapManager::getTileAt(b->m_left, b->m_top) != 203) {
                switch (b->stage()) {
                case Stage::DamagedStage:
                    for(int x = b->m_left; x < (b->m_width + b->m_left); x+=6) {
                        for(int y = b->m_top; y < (b->m_height + b->m_top); y+=6) {
                            int t = MapManager::getTileAt(x, y);
                            MapManager::setTileAt(x, y, t - 4);
                        }
                    }
                    break;
                case Stage::HeavyDamagedStage:
                    for(int x = b->m_left; x < (b->m_width + b->m_left); x+=6) {
                        for(int y = b->m_top; y < (b->m_height + b->m_top); y+=6) {
                            int t = MapManager::getTileAt(x, y);
                            MapManager::setTileAt(x, y, t - 8);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            return false;
        }

        // check for damage
        std::vector<Vec2f> hitlocs;
        int damage = ProjectileManager::getCollisionDamage(b->m_collision_rect, mask, hitlocs);
        Stage current = b->stage();
        b->m_life -= damage;
        if (b->m_life <= 0) {
            int idx = 0;
            for(int x = b->m_left; x < (b->m_width + b->m_left); x+=6) {
                for(int y = b->m_top; y < (b->m_height + b->m_top); y+=6) {
                    MapManager::setTileAt(x, y, 203);
                    idx++;
                    if ((idx % 2) == 0) {
                        if ((rand() % 10) < 7) EffectManager::createSmallExplosion(Vec2f(x + 2 + (rand() % 3), y + 2 + (rand() % 3)), rand() % 20);
                        else EffectManager::createExplosionBig(Vec2f(x - 3 + (rand() % 3), y - 3 + (rand() % 3)));
                    }
                }
            }
            return false;
        } else if (b->stage() != current) {
            for(int x = b->m_left; x < (b->m_width + b->m_left); x+=6) {
                for(int y = b->m_top; y < (b->m_height + b->m_top); y+=6) {
                    int t = MapManager::getTileAt(x, y);
                    MapManager::setTileAt(x, y, t + 4);
                }
            }
        } else {
            if (damage > 0) {
                b->m_flash.reset(5);
                for(auto p : hitlocs) {
                    EffectManager::createHit(p - Vec2f(3.5f, 3.5f));
                }
            }
        }

        // decrement counter and check for spawns
        if (b->m_spawn_count < 2) {
            b->m_spawn_timer--;
            if (b->m_spawn_timer <= 0) {
                b->m_spawn_timer = asCounts(2.33f) + (rand() % asCounts(1.0f));
                static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::LowWall, Terrain::DestructableMetal}); // todo: make this a static for EnemyMech
                if (!b->m_checks_pathfinding || Pathfinding::canReach(b->m_spawn, Camera::center(), mask)) {
                    if (b->m_spawns_tanks) {
                        EnemyTank * m = Enemy::createTank(b->m_spawn);
                        m->setMissiles();
                        ++b->m_spawn_count;
                        auto ptr = &b->m_spawn_count;
                        auto ptr2 = barracks_data + b->m_barracks_index;
                        m->setDropsCash(*ptr2 < barracksMaxMoneyDrops);
                        m->setDeactivateCallback([=](){
                            --(*ptr);
                            if (*ptr2 < 5) {
                                ++(*ptr2);
                            }
                        });
                    } else {
                        EnemyMech * m = Enemy::createMech(b->m_spawn);
                        ++b->m_spawn_count;
                        auto ptr = &b->m_spawn_count;
                        auto ptr2 = barracks_data + b->m_barracks_index;
                        m->setDropsCash(*ptr2 < barracksMaxMoneyDrops);
                        m->setDeactivateCallback([=](){
                            --(*ptr);
                            if (*ptr2 < 5) {
                                ++(*ptr2);
                            }
                        });
                    }
                }
            }
        }

        return true;
    });
}

void Barracks::draw()
{
    s_barracks.iterate([](Barracks * b){
        if (!b->m_flash.ready()) {
            Vec2f p = Camera::worldToScreen(Vec2f(b->m_left, b->m_top));
            Helpers::drawNotchedRect(p.x(), p.y(), b->m_width, b->m_height, 10);
        }
        return true;
    });
}

int8_t *Barracks::getLifePtr()
{
    return &m_life;
}

Barracks *Barracks::getBarracksAt(Vec2i loc)
{
    loc *= 6;
    Barracks * out = nullptr;
    s_barracks.iterate([&](Barracks * b) {;
        if (b->m_collision_rect.contains(loc.x(), loc.y())) {
            out = b;
        }
        return true;
    });
    return out;
}
