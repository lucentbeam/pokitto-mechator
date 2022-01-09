#ifndef ENEMYMECH_H
#define ENEMYMECH_H

#include <functional>
#include <stdint.h>

#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

class Enemy;

class EnemyMech
{
    Rect m_rect;
    Vec2f m_velocity;
    int8_t m_life = 3;
    uint8_t m_damage_frames = 0;

    enum Mode { Walking, Preparing } status = Mode::Walking;

    uint16_t m_counter = rand() % 100;

    std::function<void()> m_on_deactivate;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyMech() : m_rect(0, 0, 5, 5), m_velocity({0, 0}) {}

    void setup(const Vec2f &pos) { m_rect.setCenter(pos.x(), pos.y()); m_life = 3; m_on_deactivate = std::function<void()>(); }

    int8_t life() const { return m_life; }

    bool update(float dt);

    void setDeactivateCallback(std::function<void()> on_deactivate) { m_on_deactivate = on_deactivate; }

    void draw();
};

#endif // ENEMYMECH_H
