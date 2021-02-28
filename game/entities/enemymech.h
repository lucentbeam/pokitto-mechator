#ifndef ENEMYMECH_H
#define ENEMYMECH_H

#include "game/entities/projectile.h"
#include "game/utilities/vec.h"
#include "game/physics/regionmanager.h"

#include "core/utilities/objectpool.h"

class Enemy;
class EnemyMech;

class EnemyMech
{
    RegionRect m_rect;
    Vec2f m_velocity;
    uint8_t m_life;

    enum Mode { Walking, Preparing } status = Mode::Walking;

    uint16_t m_counter = rand() % 100;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyMech(float x, float y) : m_rect(x, y, 5, 5), m_velocity({0, 0}) {
        m_rect.parent = this;
    }
};

class Enemy
{
public:

    static void updateMechs(float dt);

    static void drawMechs(RenderSystem * renderer);
};

#endif // ENEMYMECH_H
