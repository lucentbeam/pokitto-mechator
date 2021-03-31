#ifndef STEERING_H
#define STEERING_H

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "core/utilities/vec.h"
#include "game/physics/collisionmanager.h"
#include "game/utilities/helpers.h"

class Steering {
    Vec2f m_pos;
    Vec2f m_facing = Vec2f(1.0f, 0.0f);
    Vec2f m_aim = Vec2f(1.0f, 0.0f);
    const Vec2f m_size;
    Rect m_rect;
    float m_max_speed;
    const float m_max_speed_reference;
    const float m_cornering;
    const uint16_t m_collisions;
    const float m_friction;
    bool m_moving, m_brake;
    float m_current_speed;
public:
    Steering(float x, float y, float speed, float cornering, const std::initializer_list<uint8_t> collisions, float width, float height, float friction = 1.0f) :
        m_pos(x,y),
        m_size(width,height),
        m_rect(x - width/2, y - height/2, width, height),
        m_max_speed(speed),
        m_max_speed_reference(speed),
        m_cornering(cornering),
        m_collisions(Helpers::getMask(collisions)),
        m_friction(friction)
    {

    }

    void update(float dt, float x, float y);

    void stop() { m_current_speed = 0.0f; }

    void copyPosition(const Steering &other);

    bool moving() const { return m_moving; }
    void setBrake(bool brake) { m_brake = brake; }
    Vec2f pos() const { return m_pos; }
    void setPos(const Vec2f &pos) { m_pos = pos; m_facing = Vec2f(0.0f, 1.0f); }
    Rect rect() const { return m_rect; }
    Vec2f facing() const { return m_facing; }
    Vec2f aim() const { return m_aim; }
    Vec2f vel() const { return m_facing * m_current_speed; }
    uint8_t rotation_frame() const;

    void scaleMaxSpeed(float val) { m_max_speed = m_max_speed_reference * val; }
};

#endif // STEERING_H
