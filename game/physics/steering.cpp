#include "steering.h"

void Steering::update(float dt, float x, float y, float speed_mult, bool instant) {
    m_moving = false;
    if ((std::fabs(x) > 0.01f || std::fabs(y) > 0.01f)) {
        m_moving = true;
        m_aim = Vec2f(x,y);
        if (m_aim.dot(m_facing) < 0) {
            Vec2f tmp = m_facing.rot90();
            if (m_aim.dot(tmp) < 0) {
                tmp = tmp * -1;
            }
            m_aim = tmp;
        }
        Vec2f dp = m_aim - m_facing;
        m_facing += dp * (config->cornering * dt / 0.016f);
        float len = m_facing.length();
        if (len > 0) {
            m_facing /= len;
        }
        if (instant) {
            m_current_speed = m_max_speed * speed_mult;
        } else {
            float s = m_max_speed * speed_mult - m_current_speed;
            m_current_speed += s * (dt / 0.2f);
            if (m_current_speed > m_max_speed * speed_mult) m_current_speed = m_max_speed * speed_mult;
        }
    } else {
        m_current_speed *= (1.0f - config->friction);
    }
    m_pos = CollisionManager::resolveMovement(m_pos, m_facing * m_current_speed * dt, config->collisions, {float(config->size.w), float(config->size.h)});
}

void Steering::copyPosition(const Steering &other) {
    m_pos = other.m_pos;
}

uint8_t Steering::rotation_frame(float frame_count) const {
    return getRotationFrame(m_facing, frame_count);
}

uint8_t Steering::getRotationFrame(const Vec2f dir, float frame_count)
{
    float anglePerFrame = 180.0f / frame_count;
    float angle = std::atan2(dir.y(), std::abs(dir.x())) * 180.0f / 3.1415926f;
    angle = angle + 90.0f + anglePerFrame / 2.0f; // shift atan2 to [0,180]
    return uint8_t(angle / 180.0f * frame_count);
}
