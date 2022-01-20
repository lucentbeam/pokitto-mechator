#ifndef ENEMYLASERS_H
#define ENEMYLASERS_H
#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

class Enemy;

class EnemyLasers
{
    Vec2i m_pos;
    bool vertical;
    int size;

    uint16_t m_counter = 0;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:
    EnemyLasers() :m_pos(0, 0) {}

    void setup(const Vec2f &pos, bool vert, int node, int sz);
    bool update(float dt);
    void draw() const;
};

#endif // ENEMYLASERS_H
