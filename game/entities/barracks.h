#ifndef BARRACKS_H
#define BARRACKS_H

#include "game/utilities/vec.h"
#include "core/utilities/rect.h"
#include "game/entities/enemymech.h"

#include <list>

class Barracks
{
    enum Stage { DefaultStage, DamagedStage, HeavyDamagedStage };

    static ObjectPool<Barracks, 6> s_barracks;

    static int8_t s_max_life;

    Vec2f m_spawn{0, 0};
    uint16_t m_top, m_left;
    uint8_t m_width, m_height;

    int8_t m_life = s_max_life;

    Rect m_collision_rect;

    int16_t m_spawn_timer = 160;

    Stage stage() const { return m_life > s_max_life * 2 / 3 ? DefaultStage : m_life > s_max_life / 3 ? DamagedStage : HeavyDamagedStage; }

public:

    void config(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height);

    static void create(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height);

    static void update(float dt);

    static void draw(RenderSystem * renderer);
};

template <int l, int t, int w, int h>
void spawnBarracks(const Vec2f &loc) {
    Barracks::create(loc, l, t, w, h);
}

#endif // BARRACKS_H
