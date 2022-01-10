#include "enemy.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"
#include "game/entities/effects.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"

int Mine::timer = 0;

ObjectPool<EnemyMech,10> Enemy::s_mechs;

ObjectPool<EnemyTank,3> Enemy::s_tanks;

ObjectPool<EnemyTurret,4> Enemy::s_turrets;

ObjectPool<EnemyBomber,2> Enemy::s_bombers;

ObjectPool<Mine, 20> Enemy::s_mines;

EnemyMech * Enemy::createMech(const Vec2f &pos)
{
    auto m = s_mechs.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
    return m;
}

void Enemy::updateMechs(float dt)
{
    EnemyMech * start = s_mechs.objects();
    int i = s_mechs.objectCount()-1;
    while (i >= 0) {
        if (!(start + i)->update(dt)) {
            if ((start + i)->m_on_deactivate) {
                (start+i)->m_on_deactivate();
            }
            s_mechs.deactivate(i);
        }
        --i;
    }
}

void Enemy::drawMechs()
{
    EnemyMech * start = s_mechs.objects();
    for (int i = 0; i < s_mechs.objectCount(); ++i) {
        (start + i)->draw();
    }
}

EnemyTank *Enemy::createTank(const Vec2f &pos)
{
    auto m = s_tanks.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
    return m;
}

EnemyTurret *Enemy::createTurret(const Vec2f &pos)
{
    auto m = s_turrets.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
    return m;
}

EnemyBomber *Enemy::createBomber(const Vec2f &pos)
{
    auto m = s_bombers.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
    return m;
}

void Enemy::spawnTank(const Vec2f &pos)
{
    for(int i = 0; i < s_tanks.objectCount(); ++i) {
        if ((s_tanks.objects() + i)->isFrom(pos)) {
            return;
        }
    }
    createTank(pos);
}

void Enemy::spawnBomber(const Vec2f &pos)
{
    createBomber(pos);
}

void Enemy::spawnTurret(const Vec2f &pos)
{
    createTurret(pos);
}

void Enemy::spawnMine(const Vec2f &pos)
{
    if (MapManager::getTileAt(pos.x(), pos.y()) != MapManager::UnexplodedOrdinance) return;
    auto m = s_mines.activateNext();
    if (m != nullptr) {
        m->pos = pos;
        m->timer = (rand() % 30);
    }
}

void Enemy::updateTanks(float dt)
{
    EnemyTank * start = s_tanks.objects();
    int i = s_tanks.objectCount()-1;
    while (i >= 0) {
        if (!(start + i)->update(dt)) {
            if ((start + i)->m_on_deactivate) {
                (start+i)->m_on_deactivate();
            }
            s_tanks.deactivate(i);
        }
        --i;
    }
}

void Enemy::drawTanks()
{
    EnemyTank * start = s_tanks.objects();
    for (int i = 0; i < s_tanks.objectCount(); ++i) {
        (start + i)->draw();
    }
}

void Enemy::updateTurrets(float dt)
{
    EnemyTurret * start = s_turrets.objects();
    int i = s_turrets.objectCount()-1;
    while (i >= 0) {
        if (!(start + i)->update(dt)) {
            if ((start + i)->m_on_deactivate) {
                (start+i)->m_on_deactivate();
            }
            s_turrets.deactivate(i);
        }
        --i;
    }
}

void Enemy::drawTurrets()
{
    EnemyTurret * start = s_turrets.objects();
    for (int i = 0; i < s_turrets.objectCount(); ++i) {
        (start + i)->draw();
    }
}

void Enemy::updateBombers(float dt)
{
    EnemyBomber * start = s_bombers.objects();
    int i = s_bombers.objectCount()-1;
    while (i >= 0) {
        if (!(start + i)->update(dt)) {
            s_bombers.deactivate(i);
        }
        --i;
    }
}

void Enemy::drawBombers()
{
    EnemyBomber * start = s_bombers.objects();
    for (int i = 0; i < s_bombers.objectCount(); ++i) {
        (start + i)->draw();
    }
}

void Enemy::updateMines(float dt)
{
    Mine::timer++;
    Rect player = Player::bounds();
    player.grow(3, 3);
    s_mines.iterate([&](Mine *m) {
        if (player.contains(m->pos)) {
            MapManager::setTileAt(m->pos.x(), m->pos.y(), MapManager::SpecialTiles::ExplodedOrdinance);
            ProjectileManager::create(m->pos, {0, 0}, 10, 0.1)->setDamage(6)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget, EnemyTarget});
            EffectManager::create(m->pos - Vec2f(3,3), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
            return true;
        }
        return !Camera::inActiveZone(m->pos);
    });
}

void Enemy::drawMines()
{
    for(int i = 0; i < s_mines.objectCount(); ++i) {
        if (((Mine::timer + (s_mines.objects() + i)->timer) % 120) <= 10) {
            Vec2f p = Camera::worldToScreen((s_mines.objects() + i)->pos);
            RenderSystem::pixel(p.x() + 2, p.y() + 2, 16);
        }
    }
}

void Enemy::update(float dt)
{
    updateMechs(dt);
    updateTanks(dt);
    updateTurrets(dt);
    updateBombers(dt);
    updateMines(dt);
}

void Enemy::draw()
{
    drawMines();

    drawMechs();
    drawTanks();
    drawTurrets();

    drawBombers();
}

void Enemy::drawAir()
{
    EnemyBomber * start = s_bombers.objects();
    for (int i = 0; i < s_bombers.objectCount(); ++i) {
        (start + i)->drawAir();
    }
}
