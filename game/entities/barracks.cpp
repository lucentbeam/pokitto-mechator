#include "barracks.h"
#include "projectile.h"
#include "game/utilities/helpers.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/effects.h"
#include "game/sprites.h"

#include "game/physics/collisionmanager.h"
#include "game/physics/pathfinding.h"

#include "game/maps/worldtiles.h"
#include "game/entities/pickups.h"

#include "game/constants.h"
#include "game/funcs.h"

SimplePool<Barracks, 6> Barracks::s_barracks;

int8_t Barracks::s_max_life = 15;

#ifdef DEBUGS
#include <iostream>
#endif

void Barracks::setTiles(int t, bool offset)
{
    int til = t;
    for(int x = m_left; x < (m_width + m_left); x+=6) {
        for(int y = m_top; y < (m_height + m_top); y+=6) {
            if (offset) {
                til = MapManager::getTileAt(x, y);
                til += t;
            }
            MapManager::setTileAt(x, y, til);
            if (t == 203 && (int(x/6)+int(y/6)) % 2 == 0) {
                Pickups::spawnDollar(Vec2f(x + 3, y + 3));
            }
        }
    }
}

void Barracks::config(const Vec2f &spawn, int left, int top, uint8_t width, uint8_t height)
{
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

    m_spawntype = MechSpawn;

    // If this stuff ever changes, double-check the implementation in TiledMapReader!!!
    int idx = int(left)  + int(top) * 1000;
    for(int i = 0; i < barracks_count; ++i) {
        if (idx == barracks_indices[i]) {
            m_barracks_index = i;
            return;
        }
    }
#ifdef DEBUGS
    std::cout << "did not find barracks for " << idx << std::endl;
#endif
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
    setSpawnType(TankSpawn);
}

void Barracks::setSpawnType(Barracks::SpawnType s)
{
    m_spawntype = s;
}

void Barracks::create(const Vec2i &spawn, int left, int top, uint8_t width, uint8_t height)
{    
    if (getBarracksAt({left + width/2.0f, top + height/2.0f}) != nullptr) return;
    if (MapManager::getTileAt(left * 6 + 3, top * 6 + 3) == 203) return;
    Barracks * b = s_barracks.activateNext();
    if (b == nullptr) return;
    b->config(spawn, left, top, width, height);
}

void Barracks::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Targets::GroundTarget, Targets::EnemyTarget});
    s_barracks.iterate([&](Barracks * b) {

        b->m_flash.update();

        // check if out of range. if so: deactivate and restore and damaged tiles
        if (b->m_destroy_out_of_range && !Camera::inActiveZone(b->m_spawn)) {
            if (b->stage() != Stage::DefaultStage && MapManager::getTileAt(b->m_left, b->m_top) != 203) {
                b->setTiles(b->stage() == Stage::DamagedStage ? -4 : -8, true);
            }
            return false;
        }

        // check for damage
        std::vector<Vec2f> hitlocs;
        int damage = ProjectileManager::getCollisionDamage(b->m_collision_rect, mask, hitlocs);
        Stage current = b->stage();
        b->m_life -= damage;
        if (b->m_life <= 0) {
            b->setTiles(203, false);
            onBarracksExplode();
            return false;
        } else if (b->stage() != current) {
            b->setTiles(4, true);
        } else if (damage > 0) {
            b->m_flash.reset(5);
            for(auto p : hitlocs) EffectManager::createHit(p - Vec2f(3.5f, 3.5f));
        }
        bool force_spawn = damage > 0;
        if (force_spawn && b->m_spawn_timer < asCounts(0.5f)) {
            b->m_spawn_timer = 0;
        }

        b->m_spawn_timer--;
        if (b->m_spawn_timer <= 0) {
            static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::LowWall, Terrain::DestructableMetal}); // todo: make this a static for EnemyMech
            if (force_spawn || !b->m_checks_pathfinding || Pathfinding::canReach(b->m_spawn, Camera::center(), mask)) {
                b->m_spawn_timer = asCounts(1.0f) + (rand() % asCounts(2.0f));
                if (b->m_spawntype == MechSpawn) {
                    EnemyMech * m = Enemy::createMech(b->m_spawn);
                    if (m != nullptr) m->setDropsCash(false);
                } else if (b->m_spawntype == TankSpawn) {
                    EnemyTank * m = Enemy::createTank(b->m_spawn);
                    if (m != nullptr) m->setDropsCash(false);
                } else if (b->m_spawntype == HeliSpawn) {
                    Enemy::createHelicopter(b->m_spawn);
                }
            } else {
                b->m_spawn_timer = asCounts(1.0f);
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

int8_t Barracks::getLife()
{
    return m_life;
}

void Barracks::setLife(int8_t v)
{
    Stage current = stage();
    m_life = v;
    if (stage() != current) {
        for(int x = m_left; x < (m_width + m_left); x+=6) {
            for(int y = m_top; y < (m_height + m_top); y+=6) {
                int t = MapManager::getTileAt(x, y);
                MapManager::setTileAt(x, y, t + 4);
            }
        }
    }
}

bool Barracks::isDestroyed(int lx, int ly)
{
    return MapManager::getTileAt(lx * 6 + 3, ly * 6 + 3) == 203;
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

#ifdef DEBUGS
void Barracks::queryActive()
{
    s_barracks.iterate([&](Barracks * b) {
        std::cout << "barracks at " << b->m_left / 6 << " " << b->m_top / 6 << std::endl;
        return true;
    });
}
#endif
