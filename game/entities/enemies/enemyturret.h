#ifndef ENEMYTURRET_H
#define ENEMYTURRET_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

class Enemy;

class EnemyTurret
{
    Vec2f m_aim, m_smoothaim, m_pos;
    int8_t m_life = 3;
    uint8_t m_damage_frames = 0;

    uint16_t m_counter = rand() % 100;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyTurret() : m_aim(0, 0), m_smoothaim(0,0), m_pos(0, 0) {}

    void setup(const Vec2f &pos);

    int8_t life() const { return m_life; }

    bool update(float dt);
    void draw() const;
};

#endif // ENEMYTURRET_H
