#ifndef BARRACKS_H
#define BARRACKS_H

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"
#include "game/entities/enemy.h"
#include "game/utilities/playerstats.h"
#include "game/utilities/ticker.h"

#include <list>

class Barracks
{
public:

    enum SpawnType { MechSpawn, TankSpawn, HeliSpawn, TurretSpawn };

private:
    enum Stage { DefaultStage, DamagedStage, HeavyDamagedStage };

    static SimplePool<Barracks, 6> s_barracks;

    static int8_t s_max_life;

    Vec2f m_spawn{0, 0};
    int16_t m_top, m_left;
    uint8_t m_width, m_height;

    int8_t m_life = s_max_life;

    Rect m_collision_rect;

    int16_t m_spawn_timer = 160;
    int8_t m_spawn_count = 0;
    int16_t m_barracks_index = 0;
    GenericTicker m_flash;
    SpawnType m_spawntype;
    bool m_checks_pathfinding, m_destroy_out_of_range;

    Stage stage() const { return m_life > s_max_life * 2 / 3 ? DefaultStage : m_life > s_max_life / 3 ? DamagedStage : HeavyDamagedStage; }

public:

    void config(const Vec2f &spawn, int left, int top, uint8_t width, uint8_t height);

    void disablePathfindingChecks();

    void disableDestroyOutOfRange();

    void setSpawnsTanks();
    void setSpawnType(SpawnType s);

    static void create(const Vec2i &spawn, int left, int top, uint8_t width, uint8_t height);

    static void update(float dt);

    static void draw();    

    int8_t * getLifePtr();
    int8_t getLife();
    void setLife(int8_t v);

    static bool isDestroyed(int lx, int ly);
    static Barracks * getBarracksAt(Vec2i loc);

    static void clear() { s_barracks.clear(); }
};

template <int l, int t, int w, int h>
void spawnBarracks(const Vec2i &loc) {
    Barracks::create(loc, l, t, w, h);
}

#endif // BARRACKS_H
