#ifndef STEERING_H
#define STEERING_H

#include <algorithm>
#include <cmath>

class Steering {
    float m_x, m_y;
    float m_fx = 1.0f, m_fy = 0.0f;
    const float m_speed;
    const float m_cornering;
    const float m_friction;
    bool m_moving;
    float m_interia;
public:
    Steering(float x, float y, float speed, float cornering, float friction = 1.0f) : m_x(x), m_y(y), m_speed(speed), m_cornering(cornering), m_friction(friction) {}

    void update(float dt, float x, float y);

    void copyPosition(const Steering &other);

    bool moving() const { return m_moving; }
    float x() const { return m_x; }
    float y() const { return m_y; }
    float fx() const { return m_fx; }
    float fy() const { return m_fy; }
    uint8_t rotation_frame() const;
};

#endif // STEERING_H
