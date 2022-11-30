#include "enemy.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"
#include "game/entities/effects.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"
#include "core/audiosystem.h"

int Mine::timer = 0;

ObjectPool<EnemyMech,12> Enemy::s_mechs;

ObjectPool<EnemyTank,5> Enemy::s_tanks;

ObjectPool<EnemyBoat,4> Enemy::s_boats;

ObjectPool<EnemyTurret,6> Enemy::s_turrets;

ObjectPool<EnemyBomber,3> Enemy::s_bombers;

ObjectPool<EnemyHelicopter,2> Enemy::s_helis;

ObjectPool<Mine, 14> Enemy::s_mines;

ObjectPool<WaterMine, 5> Enemy::s_watermines;

ObjectPool<EnemyLasers,4> Enemy::s_lasers;

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
    static int spacer = 0;
    spacer++;
    while (i >= 0) {
        if (!(start + i)->update(dt, (i % 2) == (spacer % 2))) {
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

EnemyTurret *Enemy::createTurretDisabled(const Vec2f &pos)
{
    auto m = s_turrets.activateNext();
    if (m != nullptr) {
        m->setup(pos);
        m->setDisabled(true);
    }
    return m;
}

EnemyTurret *Enemy::getTurretAt(Vec2i loc)
{
    loc *= 6;
    EnemyTurret * t = nullptr;
    s_turrets.iterate([&](EnemyTurret * tur) {
        if (tur->life() > 0 && tur->rect().contains(loc.x(), loc.y())) {
            t = tur;
        }
        return false;
    });
    return t;
}

EnemyTurret *Enemy::getTurretAtLoc(Vec2f loc)
{
    EnemyTurret * t = nullptr;
    s_turrets.iterate([&](EnemyTurret * tur) {
        if (tur->life() > 0 && tur->rect().contains(loc.x(), loc.y())) {
            t = tur;
        }
        return false;
    });
    return t;
}

EnemyBomber *Enemy::createBomber(const Vec2f &pos)
{
    auto m = s_bombers.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
    return m;
}

EnemyHelicopter *Enemy::createHelicopter(const Vec2f &pos)
{
    auto m = s_helis.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
    return m;
}

EnemyBoat *Enemy::createBoat(const Vec2f &pos)
{
    auto b = s_boats.activateNext();
    if (b != nullptr) {
        b->setup(pos);
    }
    return b;
}

void Enemy::spawnMech(const Vec2i &pos)
{
    createMech(pos);
}

void Enemy::spawnTank(const Vec2i &pos)
{
    for(int i = 0; i < s_tanks.objectCount(); ++i) {
        if ((s_tanks.objects() + i)->isFrom(pos)) {
            return;
        }
    }
    createTank(pos);
}

void Enemy::spawnBomber(const Vec2i &pos)
{
    createBomber(pos);
}

void Enemy::spawnTurret(const Vec2i &pos)
{
    createTurret(pos);
}

void Enemy::spawnTurretDisabled(const Vec2i &pos)
{
    createTurretDisabled(pos);
}

void Enemy::spawnMine(const Vec2i &pos)
{
    if (MapManager::getTileAt(pos.x(), pos.y()) != SpecialTiles::UnexplodedOrdinance) return;
    auto m = s_mines.activateNext();
    if (m != nullptr) {
        m->pos = pos + Vec2i(3, 3);
        m->beep_freq = (rand() % 30);
    }
}

void Enemy::spawnHelicopter(const Vec2i &pos)
{
    createHelicopter(pos);
}

void Enemy::createLasers(const Vec2i &pos, bool vertical, int sz)
{
    auto m = s_lasers.activateNext();
    if (m != nullptr) {
        m->setup(pos, vertical, sz);
    }
}

void Enemy::spawnWaterMine(const Vec2i &pos, int w, int h)
{
    auto m = s_watermines.activateNext();
    if (m != nullptr) {
        m->pos = pos;
        m->offset = Vec2i(rand() % w, rand() % h) * 6;
        m->bob_freq = (rand() % 30);
    }
}

void Enemy::spawnBoat(const Vec2i &pos)
{
    createBoat(pos);
}

void Enemy::updateTanks(float dt)
{
    EnemyTank * start = s_tanks.objects();
    int i = s_tanks.objectCount()-1;
    while (i >= 0) {
        if (!(start + i)->update(dt)) {
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
    s_turrets.iterate([&](EnemyTurret * h) {
        return !h->update(dt);
    });
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

    EnemyBomber::drop_counter++;
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

void Enemy::updateHelis(float dt)
{
    s_helis.iterate([&](EnemyHelicopter * h) {
        return !h->update(dt);
    });
}

void Enemy::drawHelis()
{
    EnemyHelicopter * start = s_helis.objects();
    for (int i = 0; i < s_helis.objectCount(); ++i) {
        (start + i)->draw();
    }
}

void Enemy::updateMines(float dt)
{
    Mine::timer++;
    bool skip = Player::mode() == PlayerMode::HelicopterMode;
    Rect player = Player::bounds();
    player.grow(3, 3);
    s_mines.iterate([&](Mine *m) {
        if (!skip && player.contains(m->pos)) {
            AudioSystem::play(sfxExplosionSmall);
            MapManager::setTileAt(m->pos.x(), m->pos.y(), SpecialTiles::ExplodedOrdinance);
            ProjectileManager::create(m->pos, {0, 0}, 10, 0.1)->setDamage(6)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget, EnemyTarget});
            EffectManager::createExplosionBig(m->pos - Vec2f(6,6));
            return true;
        }
        return !Camera::inActiveZone(m->pos - Vec2f(3, 3));
    });
    if (Player::mode() == PlayerMode::BoatMode) {
        s_watermines.iterate([&](WaterMine *m) {
            if (player.contains(m->pos + m->offset)) {
                AudioSystem::play(sfxExplosionSmall);
                ProjectileManager::create(m->pos + m->offset, {0, 0}, 10, 0.1)->setDamage(6)->setIgnoreWalls()->setTargetMask({PlayerTarget, GroundTarget, EnemyTarget});
                EffectManager::createExplosionBig(m->pos + m->offset - Vec2f(6,6));
                return true;
            }
            return !Camera::inActiveZone(m->pos);
        });
    }
    s_lasers.iterate([&](EnemyLasers * l) {
        return !l->update(dt);
    });
}

void Enemy::drawMines()
{
    for(int i = 0; i < s_mines.objectCount(); ++i) {
        if (((Mine::timer + (s_mines.objects() + i)->beep_freq) % 120) <= 10) {
            Vec2f p = Camera::worldToScreen((s_mines.objects() + i)->pos);
            RenderSystem::pixel(p.x() - 1, p.y() - 1, 16);
        }
    }
    for(int i = 0; i < s_watermines.objectCount(); ++i) {
        bool bob = ((Mine::timer + (s_watermines.objects() + i)->bob_freq) % 130) <= 65;
        Vec2f p = Camera::worldToScreen((s_watermines.objects() + i)->pos + (s_watermines.objects() + i)->offset);
        RenderSystem::sprite(p.x(), p.y() - (bob ? 1 : 0), enemy_watermine, 0);
    }
    s_lasers.iterate([&](EnemyLasers * l) {
        l->draw();
        return false;
    });
}

void Enemy::updateBoats(float dt)
{
    EnemyBoat * start = s_boats.objects();
    int i = s_boats.objectCount()-1;
    while (i >= 0) {
        if (!(start + i)->update(dt)) {
            s_boats.deactivate(i);
        }
        --i;
    }
}

void Enemy::drawBoats()
{
    EnemyBoat * start = s_boats.objects();
    for (int i = 0; i < s_boats.objectCount(); ++i) {
        (start + i)->draw();
    }
}

void Enemy::update(float dt)
{
    updateMechs(dt);
    updateTanks(dt);
    updateBoats(dt);
    updateTurrets(dt);
    updateBombers(dt);
    updateMines(dt);    
    updateHelis(dt);
}

void Enemy::draw()
{
    drawMines();

    drawMechs();
    drawTanks();
    drawBoats();
    drawTurrets();

    drawBombers();
    drawHelis();
}

void Enemy::drawAir()
{
    EnemyBomber * start = s_bombers.objects();
    for (int i = 0; i < s_bombers.objectCount(); ++i) {
        (start + i)->drawAir();
    }
    EnemyHelicopter * starth = s_helis.objects();
    for (int i = 0; i < s_helis.objectCount(); ++i) {
        (starth + i)->drawAir();
    }
}

void Enemy::clearAll()
{
    s_mechs = ObjectPool<EnemyMech, 12>();
    s_tanks = ObjectPool<EnemyTank, 5>();
    s_boats = ObjectPool<EnemyBoat, 4>();
    s_turrets = ObjectPool<EnemyTurret, 6>();
    s_bombers = ObjectPool<EnemyBomber, 3>();
    s_helis = ObjectPool<EnemyHelicopter, 2>();
    s_lasers = ObjectPool<EnemyLasers, 4>();
    s_mines = ObjectPool<Mine, 14>();
    s_watermines = ObjectPool<WaterMine, 5>();
}
