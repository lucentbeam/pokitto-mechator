#ifndef STEERING_H
#define STEERING_H

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "core/utilities/vec.h"
#include "game/physics/collisionmanager.h"
#include "game/utilities/helpers.h"

struct SteeringConfig {
    const Bounds size;
    const float max_speed;
    const float cornering;
    const float friction;
    const uint16_t collisions;

    constexpr SteeringConfig( float speed, float cornering, uint16_t collisions, float width, float height, float friction = 1.0f) :
        size(width, height),
        max_speed(speed),
        cornering(cornering),
        friction(friction),
        collisions(collisions)
    {

    }
};

class Steering {
    Vec2f m_pos;
    Vec2f m_facing = Vec2f(1.0f, 0.0f);
    Vec2f m_aim = Vec2f(1.0f, 0.0f);

    const SteeringConfig * config;

    float m_current_speed;
    uint8_t m_max_speed;
    bool m_moving;
public:
    Steering(float x, float y, const SteeringConfig * conf) :
        m_pos(x,y),
        config(conf),
        m_max_speed(conf->max_speed)
    {

    }

    void update(float dt, float x, float y, float speed_mult = 1.0f, bool instant = false);

    void stop() { m_current_speed = 0.0f; }

    void setSteering(const SteeringConfig * other) {
        config = other;
    }

    void copyPosition(const Steering &other);

    bool moving() const { return m_moving; }
    Vec2f pos() const { return m_pos; }
    void setPos(const Vec2f &pos) { m_pos = pos; m_facing = Vec2f(0.0f, 1.0f); }
    Rect rect() const { return Rect(m_pos.x(), m_pos.y(), config->size); }
    Vec2f facing() const { return m_facing; }
    Vec2f aim() const { return m_aim; }
    Vec2f vel() const { return m_facing * m_current_speed; }
    uint8_t rotation_frame(float frame_count = 8.0f) const;

    void scaleMaxSpeed(float val) { m_max_speed = config->max_speed * val; }

    static uint8_t getRotationFrame(const Vec2f dir, float frame_count);
};

#endif // STEERING_H
