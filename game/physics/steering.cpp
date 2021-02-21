#include "steering.h"

void Steering::update(float dt, float x, float y) {
    m_moving = false;
    if (std::fabs(x) > 0.01f || std::fabs(y) > 0.01f) {
        m_moving = true;
        float dx = x - m_fx;
        float dy = y - m_fy;
        m_fx += dx * m_cornering;
        m_fy += dy * m_cornering;
        float len = std::sqrt(m_fx*m_fx + m_fy*m_fy);
        if (len > 0) {
            m_fx /= len;
            m_fy /= len;
        }
        m_interia = m_speed;
    } else {
        m_interia *= (1.0f - m_friction);
    }
    m_x += m_fx * m_interia * dt;
    m_y += m_fy * m_interia * dt;
}

void Steering::copyPosition(const Steering &other) {
    m_x = other.m_x;
    m_y = other.m_y;
}

uint8_t Steering::rotation_frame() const {
    float angle = std::atan2(m_fy, std::abs(m_fx)) * 180.0f / 3.1415926f;
    angle = angle + 90.0f + 11.25f;
    return uint8_t(angle / 180.0f * 8.0f);
}
