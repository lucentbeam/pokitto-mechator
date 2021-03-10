#ifndef BODY_H
#define BODY_H

#include "game/utilities/vec.h"

class Body
{
    Vec2f m_pos, m_vel;
public:
    Body(const Vec2f &pos, const Vec2f &vel);

    void update(float dt);

    Vec2f pos() const { return m_pos; }
};

#endif // BODY_H
