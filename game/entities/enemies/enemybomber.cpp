#include "enemybomber.h"


void EnemyBomber::setup(const Vec2f &pos)
{
    m_pos = pos;
    m_pos.setX(m_pos.x() - 70);
}

bool EnemyBomber::update(float dt)
{

}

void EnemyBomber::draw() const
{

}
