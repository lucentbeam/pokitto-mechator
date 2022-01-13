#ifndef BODY_H
#define BODY_H

#include "core/utilities/vec.h"

class Body
{
    Vec2f m_pos, m_vel;
public:
    Body() : Body({0, 0}, {0, 0}) {}
    Body(const Vec2f &pos, const Vec2f &vel);

    void update(float dt);

    void steerToward(const Vec2f &target);

    Vec2f pos() const { return m_pos; }

    void addVel(const Vec2f &vel);

    Vec2f vel() const { return m_vel; }
};

#endif // BODY_H
