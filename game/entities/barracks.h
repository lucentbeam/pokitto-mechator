#ifndef BARRACKS_H
#define BARRACKS_H

//#include "core/utilities/objectpool.h"
#include "game/utilities/vec.h"
#include "core/utilities/rect.h"
#include "game/entities/enemymech.h"

class Barracks
{
    static ObjectPool<Barracks, 6> s_barracks;

    Vec2f m_spawn{0, 0};
    uint16_t m_top, m_left;
    uint8_t m_width, m_height;
    Rect m_collision_rect;

public:
    void config(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height);

    static void create(const Vec2f &spawn, uint16_t left, uint16_t top, uint8_t width, uint8_t height);

    static void update(float dt);
};

template <int l, int t, int w, int h>
void spawnBarracks(const Vec2f &loc) {
    Barracks::create(loc, l, t, w, h);
}

#endif // BARRACKS_H
