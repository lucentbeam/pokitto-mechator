#include "body.h"

Body::Body(const Vec2f &pos, const Vec2f &vel) :
    m_pos(pos),
    m_vel(vel)
{

}

void Body::update(float dt)
{
    m_pos += m_vel * dt;
}

void Body::steerToward(const Vec2f &target)
{
    float s = m_vel.length();
    Vec2f dir = m_vel;
    if (s > 0) {
        dir /= s;
    }
    Vec2f tar = target - m_pos;
    float d = tar.length();
    if (d > 0) {
        tar /= d;
    }
    const float f = 0.05f;
    m_vel = dir * (1.0f - f) + tar * f;
    d = m_vel.length();
    if (d > 0) {
        m_vel /= d;
    }
    m_vel *= s;
}

void Body::addVel(const Vec2f &vel)
{
    m_vel += vel;
}
