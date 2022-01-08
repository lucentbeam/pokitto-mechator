#ifndef ENEMY_H
#define ENEMY_H

#include "core/utilities/objectpool.h"
#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

#include "game/entities/projectile.h"
#include "enemies/enemymech.h"
#include "enemies/enemytank.h"

class Enemy
{
    static ObjectPool<EnemyMech, 10> s_mechs;
    static ObjectPool<EnemyTank, 3> s_tanks;

    static void updateMechs(float dt);
    static void drawMech(EnemyMech * mech);
    static void drawMechs();

    static void updateTanks(float dt);
    static void drawTanks();

public:    
    static EnemyMech * createMech(const Vec2f &pos);
    static EnemyTank * createTank(const Vec2f &pos);

    static void spawnTank(const Vec2f &pos);

    static void update(float dt);
    static void draw();
};

#endif // ENEMY_H
