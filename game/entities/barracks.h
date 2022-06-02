#ifndef BARRACKS_H
#define BARRACKS_H

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"
#include "game/entities/enemy.h"
#include "game/utilities/playerstats.h"

#include <list>

class Barracks
{
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
    Ticker m_flash;
    bool m_checks_pathfinding, m_destroy_out_of_range, m_spawns_tanks;

    Stage stage() const { return m_life > s_max_life * 2 / 3 ? DefaultStage : m_life > s_max_life / 3 ? DamagedStage : HeavyDamagedStage; }

public:

    void config(const Vec2f &spawn, int left, int top, uint8_t width, uint8_t height);

    void disablePathfindingChecks();

    void disableDestroyOutOfRange();

    void setSpawnsTanks();

    static void create(const Vec2i &spawn, int left, int top, uint8_t width, uint8_t height);

    static void update(float dt);

    static void draw();    

    int8_t * getLifePtr();

    static Barracks * getBarracksAt(Vec2i loc);
};

template <int l, int t, int w, int h>
void spawnBarracks(const Vec2i &loc) {
    Barracks::create(loc, l, t, w, h);
}

#endif // BARRACKS_H
