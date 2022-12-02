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
#include "enemies/enemyboat.h"

constexpr int maxmechcount = 12;
constexpr int maxtankcount = 5;

struct Mine {
    Vec2f pos;
    int beep_freq;
    static int timer;
};

struct WaterMine {
    Vec2f pos;
    Vec2i offset;
    int bob_freq;
};

class Enemy
{
    static ObjectPool<EnemyMech, maxmechcount> s_mechs;
    static ObjectPool<EnemyTank, maxtankcount> s_tanks;
    static ObjectPool<EnemyBoat, 4> s_boats;
    static ObjectPool<EnemyTurret, 6> s_turrets;
    static ObjectPool<EnemyBomber, 3> s_bombers;
    static ObjectPool<EnemyHelicopter, 2> s_helis;
    static ObjectPool<EnemyLasers, 4> s_lasers;
    static ObjectPool<Mine, 14> s_mines;
    static ObjectPool<WaterMine, 7> s_watermines;

    static int s_max_tanks, s_max_mechs;

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

    static void updateBoats(float dt);
    static void drawBoats();

public:    
    static EnemyMech * createMech(const Vec2f &pos);
    static EnemyTank * createTank(const Vec2f &pos);
    static EnemyTurret * createTurret(const Vec2f &pos);
    static EnemyTurret * createTurretDisabled(const Vec2f &pos);
    static EnemyTurret * getTurretAt(Vec2i loc);
    static EnemyTurret * getTurretAtLoc(Vec2f loc);
    static EnemyBomber * createBomber(const Vec2f &pos);
    static EnemyHelicopter * createHelicopter(const Vec2f &pos);
    static EnemyBoat * createBoat(const Vec2f &pos);

    static void spawnMech(const Vec2i &pos);
    static void spawnTank(const Vec2i &pos);
    static void spawnBomber(const Vec2i &pos);
    static void spawnTurret(const Vec2i &pos);
    static void spawnTurretDisabled(const Vec2i &pos);
    static void spawnMine(const Vec2i &pos);
    static void spawnHelicopter(const Vec2i &pos);
    static void createLasers(const Vec2i &pos, bool vertical, int sz);
    static void spawnWaterMine(const Vec2i &pos, int w, int h);
    static void spawnBoat(const Vec2i &pos);

    static void update(float dt);
    static void draw();
    static void drawAir();

    static void setMaxMechs(int ct = -1, bool clear = false);
    static void setMaxTanks(int ct = -1, bool clear = false);

    static void clearAll();
};

#endif // ENEMY_H
