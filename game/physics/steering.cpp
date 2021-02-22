#include "steering.h"

void Steering::update(float dt, float x, float y) {
    m_moving = false;
    if (std::fabs(x) > 0.01f || std::fabs(y) > 0.01f) {
        m_moving = true;
        Vec2f dp = Vec2f(x,y) - m_facing;
        m_facing += dp * m_cornering;
        float len = m_facing.length();
        if (len > 0) {
            m_facing /= len;
        }
        m_interia = m_speed;
    } else {
        m_interia *= (1.0f - m_friction);
    }
    m_pos += m_facing * m_interia * dt;
}

void Steering::copyPosition(const Steering &other) {
    m_pos = other.m_pos;
}

uint8_t Steering::rotation_frame() const {
    float angle = std::atan2(m_facing.y(), std::abs(m_facing.x())) * 180.0f / 3.1415926f;
    angle = angle + 90.0f + 11.25f;
    return uint8_t(angle / 180.0f * 8.0f);
}
