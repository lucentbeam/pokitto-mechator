#ifndef ENEMYMECH_H
#define ENEMYMECH_H

#include <functional>
#include <stdint.h>

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

    enum Mode { Walking, Preparing } status;

    uint16_t m_counter;

    std::function<void()> m_on_deactivate;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyMech() = default;

    void setup(const Vec2f &pos) { m_pos = pos; m_life = 3; m_on_deactivate = std::function<void()>(); m_drops = true; m_counter = rand() % 30; status = Mode::Walking; }

    int8_t life() const { return m_life; }

    bool update(float dt, bool check_collisions = true);

    void setDeactivateCallback(std::function<void()> on_deactivate) { m_on_deactivate = on_deactivate; }

    void setDropsCash(bool drops) { m_drops = drops; }

    void draw();
};

#endif // ENEMYMECH_H
