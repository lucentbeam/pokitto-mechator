#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

#include "game/physics/steering.h"

const float enemyTankSpeed = 18.0f;

class Enemy;

class EnemyTank
{
    Vec2f m_aim, m_origin;
    Steering m_steering;
    int8_t m_life = 3;
    uint8_t m_damage_frames = 0;

    enum Mode { Walking, Preparing } status = Mode::Walking;

    uint16_t m_counter = rand() % 100;

    std::function<void()> m_on_deactivate;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyTank() : m_aim(0, 0), m_origin(0, 0), m_steering(0.0f, 0.0f, enemyTankSpeed, 0.04f, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 10, 10) {}

    void setup(const Vec2f &pos) { m_origin = pos; m_life = 3; m_on_deactivate = std::function<void()>(); m_steering.setPos(pos); }

    bool isFrom(const Vec2f &pos) { return (m_origin - pos).length() < 0.5f; }

    bool update(float dt);

    int8_t life() const { return m_life; }

    void setDeactivateCallback(std::function<void()> on_deactivate) { m_on_deactivate = on_deactivate; }

    void draw() const;
};

#endif // ENEMYTANK_H
