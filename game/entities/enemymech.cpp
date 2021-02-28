#include "enemymech.h"
#include "game/rendering/camera.h"
#include "game/tilesets.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"



void Enemy::updateMechs(float dt)
{
    float px = Camera::tl_x();
    float py = Camera::tl_y();
    float tx = px + 55;
    float ty = py + 44;

    static uint8_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal});

    for(auto rect : RegionRect::getInRange(px - 24, py - 24, 110+24, 88+24)) {
        EnemyMech * mech = reinterpret_cast<EnemyMech*>(rect->parent);
        mech->m_counter++;
        Vec2f dir = {tx - mech->m_rect.centerX(), ty - mech->m_rect.centerY()};
        float len = dir.length();
        if (len > 0) {
            dir = dir / len;
        }
        switch (mech->status) {
        case EnemyMech::Mode::Walking:
            if (mech->m_counter % 40 == 0) {
                Vec2f alt = {float(rand() % 100) - 50, float(rand() % 100) - 50};
                alt = alt / 50.0f;
                mech->m_velocity = (alt * 0.5 + dir * 0.5) * 15.0f;
            }
            if (mech->m_counter > 180) {
                mech->status = EnemyMech::Mode::Preparing;
                mech->m_velocity = {0, 0};
                mech->m_counter = rand() % 10;
            }
            break;
        case EnemyMech::Mode::Preparing:
            if (mech->m_counter > 60) {
                Projectile * p = ProjectileManager::create({rect->centerX(), rect->centerY()}, dir * 50.0f, 3.0);
                p->setSprite({projectile[0], projectile[1]}, 20.0);
                mech->status = EnemyMech::Mode::Walking;
                mech->m_counter = rand() % 40;
            }
            break;
        default:
            break;
        }
        Vec2f pos = {rect->centerX(), rect->centerY()};
        pos = CollisionManager::resolveMovement(pos, mech->m_velocity * dt, mask, {4,4});
        rect->move(pos.x(), pos.y());
    }
}

void Enemy::drawMechs(RenderSystem *renderer)
{
    float px = Camera::tl_x();
    float py = Camera::tl_y();
    for(auto rect : RegionRect::getInRange(px - 12, py - 12, 110+24, 88+24)) {
        auto pos = Camera::worldToScreen({rect->centerX(), rect->centerY()});
        EnemyMech * mech = reinterpret_cast<EnemyMech*>(rect->parent);
        if (mech->status == EnemyMech::Mode::Preparing) {
            int idx = mech->m_counter % 10 < 5 ? 1 : 3;
            renderer->sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
        } else {
            const int increment = 7;
            int idx = mech->m_counter % (increment*4);
            idx = idx < increment ? 0 : idx < increment*2 ? 1 : idx < increment*3 ? 2 : 1;
            renderer->sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
        }
    }
}
