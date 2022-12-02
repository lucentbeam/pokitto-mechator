#ifndef ENEMYMECH_H
#define ENEMYMECH_H

#include <functional>
#include <stdint.h>

#include "game/entities/enemies/enemyaihelper.h"
#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

class Enemy;

class EnemyMech
{
    Vec2f m_pos;
//    Bounds m_size;
    Vec2f m_velocity;
    int8_t m_life;
    uint8_t m_damage_frames;

    bool m_drops;

    AIMode status;

    uint16_t m_counter;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyMech() = default;

    void setup(const Vec2f &pos) { m_pos = pos; m_life = 1; m_drops = true; m_counter = rand() % 60; status = AIMode::Walking; m_damage_frames = 0; }

    int8_t life() const { return m_life; }

    bool update(float dt, bool check_collisions = true);

    void setDropsCash(bool drops) { m_drops = drops; }

    void draw();
};

#endif // ENEMYMECH_H
