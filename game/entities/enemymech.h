#ifndef ENEMYMECH_H
#define ENEMYMECH_H

#include "game/entities/projectile.h"
#include "game/utilities/vec.h"

#include "core/utilities/rect.h"

class Enemy;
class EnemyMech;

class EnemyMech
{
    Rect m_rect;
    Vec2f m_velocity;
    uint8_t m_life;

    enum Mode { Walking, Preparing } status = Mode::Walking;

    uint16_t m_counter = rand() % 100;

    friend Enemy; // I suppose that makes Enemy the enemy of this's enemy?
public:

    EnemyMech() : m_rect(0, 0, 5, 5), m_velocity({0, 0}) {}

    void setup(const Vec2f &pos) { m_rect.setCenter(pos.x(), pos.y()); }
};

class Enemy
{
    static ObjectPool<EnemyMech, 10> s_mechs;

public:

    static void createMech(const Vec2f &pos);

    static void updateMechs(float dt);

    static void drawMechs(RenderSystem * renderer);
};

#endif // ENEMYMECH_H
