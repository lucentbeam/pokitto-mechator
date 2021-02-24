#ifndef STEERING_H
#define STEERING_H

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "game/utilities/vec.h"
#include "game/physics/collisionmanager.h"
#include "game/utilities/helpers.h"

class Steering {
    Vec2f m_pos;
    Vec2f m_facing = Vec2f(1.0f, 0.0f);
    Vec2f m_aim = Vec2f(1.0f, 0.0f);
    const Vec2f m_size;
    const float m_speed;
    const float m_cornering;
    const uint16_t m_collisions;
    const float m_friction;
    bool m_moving;
    float m_interia;
public:
    Steering(float x, float y, float speed, float cornering, const std::initializer_list<uint8_t> collisions, float width, float height, float friction = 1.0f) :
        m_pos(x,y),
        m_size(width,height),
        m_speed(speed),
        m_cornering(cornering),
        m_collisions(Helpers::getMask(collisions)),
        m_friction(friction)
    {

    }

    void update(float dt, float x, float y);

    void copyPosition(const Steering &other);

    bool moving() const { return m_moving; }

    Vec2f pos() const { return m_pos; }
    Vec2f facing() const { return m_facing; }
    Vec2f aim() const { return m_aim; }
    Vec2f vel() const { return m_facing * m_interia; }
    uint8_t rotation_frame() const;
};

#endif // STEERING_H
