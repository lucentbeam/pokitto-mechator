#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

#include "game/physics/steering.h"
#include "game/constants.h"

class Enemy;

const SteeringConfig enemy_tank_steering(enemyTankSpeed, 0.04f,
       (1 << Terrain::Wall) | (1 << Terrain::WaterDeep) | (1 << Terrain::WaterShallow) | (1 << Terrain::DestrucableWood) | (1 << Terrain::DestructableMetal) | (1 << Terrain::LowWall),
       10, 10);

class EnemyTank
{
    std::function<void()> m_deactivate;
    Vec2f m_aim;
    Vec2i m_origin;
    Steering m_steering;
    int8_t m_life = 3;
    uint8_t m_damage_frames = 0;

    bool m_drops_cash;
    bool m_missiles;

    enum Mode : uint8_t { Walking, Preparing } status = Mode::Walking;

    uint16_t m_counter = rand() % 100;


    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyTank() : m_aim(0, 0), m_origin(0, 0), m_steering(0.0f, 0.0f, &enemy_tank_steering) {}

    void setup(const Vec2f &pos);

    bool isFrom(const Vec2f &pos) { return (Vec2f(m_origin.x(), m_origin.y()) - pos).length() < 0.5f; }

    bool update(float dt);

    void setDropsCash(bool cash) { m_drops_cash = cash; }

    void setDeactivateCallback(std::function<void()> deactivate) { m_deactivate = deactivate; }

    void setMissiles() { m_missiles = true; }

    int8_t life() const { return m_life; }

    void draw() const;
};

#endif // ENEMYTANK_H
