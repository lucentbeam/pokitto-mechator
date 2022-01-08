#include "enemy.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"
#include "game/entities/effects.h"
#include "game/entities/pickups.h"
#include "game/physics/pathfinding.h"

ObjectPool<EnemyMech,10> Enemy::s_mechs;

ObjectPool<EnemyTank,3> Enemy::s_tanks;

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

#include <string>

void Enemy::drawMech(EnemyMech *mech)
{
    auto pos = Camera::worldToScreen({mech->m_rect.centerX(), mech->m_rect.centerY()});
    if (mech->m_damage_frames > 0) {
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[4], enemy_ground[4][2]);
    } else if (mech->status == EnemyMech::Mode::Preparing) {
        int idx = mech->m_counter % 10 < 5 ? 1 : 3;
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    } else {
        const int increment = 7;
        int idx = mech->m_counter % (increment*4);
        idx = idx < increment ? 0 : idx < increment*2 ? 1 : idx < increment*3 ? 2 : 1;
        RenderSystem::sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    }
//    int i = mech->path.size();
//    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal});
//    for(auto p : mech->path) {
//        bool hit = CollisionManager::collides(p * 6 + Vec2f(3,3), mask);
//        Vec2f pos = Camera::worldToScreen(p * 6);
//        RenderSystem::drawRect(pos.x()-3, pos.y()-3, 6, 6, 38);
//        Helpers::printHorizontallyCentered(pos.x(), pos.y() - 3, std::to_string(i).c_str(), hit ? 15 : 41);
//        --i;
//    }
}

void Enemy::drawMechs()
{
    EnemyMech * start = s_mechs.objects();
    for (int i = 0; i < s_mechs.objectCount(); ++i) {
        drawMech(start + i);
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

void Enemy::spawnTank(const Vec2f &pos)
{
    for(int i = 0; i < s_tanks.objectCount(); ++i) {
        if ((s_tanks.objects() + i)->isFrom(pos)) {
            return;
        }
    }
    createTank(pos);
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

void Enemy::update(float dt)
{
    updateMechs(dt);
    updateTanks(dt);
}

void Enemy::draw()
{
    drawMechs();
    drawTanks();
}
