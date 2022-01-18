#ifndef ENEMYHELICOPTER_H
#define ENEMYHELICOPTER_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

#include "game/physics/steering.h"
#include "game/constants.h"

class Enemy;

class EnemyHelicopter
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

    EnemyHelicopter() : m_aim(0, 0), m_origin(0, 0), m_steering(0.0f, 0.0f, enemyHeliSpeed, 0.04f, {}, 16, 16) {}

    void setup(const Vec2f &pos) { m_origin = pos; m_life = 8; m_on_deactivate = std::function<void()>(); m_steering.setPos(pos); m_counter = rand() % 30; status = Mode::Walking; }

    bool isFrom(const Vec2f &pos) { return (m_origin - pos).length() < 0.5f; }

    bool update(float dt);

    void setDeactivateCallback(std::function<void()> on_deactivate) { m_on_deactivate = on_deactivate; }

    void draw() const;
    void drawAir() const;

    Steering &getSteering() { return m_steering; }
    void tick() { m_counter++; }
    int8_t * getLifePtr() { return &m_life; }
    void setMaxLife(int8_t l) { m_life = l; }
};

#endif // ENEMYHELICOPTER_H
