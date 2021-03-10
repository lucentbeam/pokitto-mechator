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
