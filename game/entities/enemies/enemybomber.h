#ifndef ENEMYBOMBER_H
#define ENEMYBOMBER_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

class Enemy;

class EnemyBomber
{
    Vec2f m_pos, m_plane_pos;
    Rect m_rect;
    int8_t m_life = 3;
    uint8_t m_damage_frames = 0;

    enum Status {
        Waiting,
        InSky,
        FlyBy
    } status;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyBomber() :m_pos(0, 0), m_rect(0, 0, 10, 10) {}

    void setup(const Vec2f &pos);
    bool update(float dt);
    void draw() const;
    void drawAir() const;
};

#endif // ENEMYBOMBER_H
