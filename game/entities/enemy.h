#ifndef ENEMY_H
#define ENEMY_H

#include "core/utilities/objectpool.h"
#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

#include "game/entities/projectile.h"
#include "enemies/enemymech.h"
#include "enemies/enemytank.h"
#include "enemies/enemyturret.h"
#include "enemies/enemybomber.h"
#include "enemies/enemyhelicopter.h"
#include "enemies/enemylasers.h"

struct Mine {
    Vec2f pos;
    int beep_freq;
    static int timer;
};

class Enemy
{
    static ObjectPool<EnemyMech, 10> s_mechs;
    static ObjectPool<EnemyTank, 3> s_tanks;
    static ObjectPool<EnemyTurret, 4> s_turrets;
    static ObjectPool<EnemyBomber, 2> s_bombers;
    static ObjectPool<EnemyHelicopter, 2> s_helis;
    static ObjectPool<EnemyLasers, 4> s_lasers;
    static ObjectPool<Mine, 20> s_mines;

    static void updateMechs(float dt);
    static void drawMechs();

    static void updateTanks(float dt);
    static void drawTanks();

    static void updateTurrets(float dt);
    static void drawTurrets();

    static void updateBombers(float dt);
    static void drawBombers();

    static void updateHelis(float dt);
    static void drawHelis();

    static void updateMines(float dt);
    static void drawMines();

public:    
    static EnemyMech * createMech(const Vec2f &pos);
    static EnemyTank * createTank(const Vec2f &pos);
    static EnemyTurret * createTurret(const Vec2f &pos);
    static EnemyBomber * createBomber(const Vec2f &pos);

    static void spawnMech(const Vec2f &pos);
    static void spawnTank(const Vec2f &pos);
    static void spawnBomber(const Vec2f &pos);
    static void spawnTurret(const Vec2f &pos);
    static void spawnMine(const Vec2f &pos);
    static void spawnHelicopter(const Vec2f &pos);
    static void createLasers(const Vec2f &pos, bool vertical, int node, int sz);

    static void update(float dt);
    static void draw();
    static void drawAir();
};


template <int t, int h>
void spawnVerticalLasers(const Vec2f &loc) {
    Enemy::createLasers(loc, true, t, h);
}

template <int l, int w>
void spawnHorizontalLasers(const Vec2f &loc) {
    Enemy::createLasers(loc, false, l, w);
}

#endif // ENEMY_H
