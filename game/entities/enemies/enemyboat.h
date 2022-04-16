#ifndef ENEMYBOAT_H
#define ENEMYBOAT_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

#include "game/physics/steering.h"
#include "game/constants.h"

class Enemy;

const SteeringConfig enemy_boat_steering(enemyBoatSpeed, 0.04f,
       ~(1 << Terrain::WaterDeep),
       10, 10, 0.95f);

class EnemyBoat
{
    Vec2f m_aim;
    Vec2i m_origin;
    Steering m_steering;
    int8_t m_life = 3;
    uint8_t m_damage_frames = 0;

    uint16_t m_counter = rand() % 100;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyBoat() : m_aim(0, 0), m_origin(0, 0), m_steering(0.0f, 0.0f, &enemy_boat_steering) {}

    void setup(const Vec2f &pos);

    bool isFrom(const Vec2f &pos) { return (Vec2f(m_origin.x(), m_origin.y()) - pos).length() < 0.5f; }

    bool update(float dt);

    int8_t life() const { return m_life; }

    void draw() const;
};

#endif // ENEMYBOAT_H
