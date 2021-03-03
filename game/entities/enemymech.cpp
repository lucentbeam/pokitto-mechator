#include "enemymech.h"
#include "game/rendering/camera.h"
#include "game/tilesets.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"
#include "game/entities/effects.h"

ObjectPool<EnemyMech,10> Enemy::s_mechs;

void Enemy::createMech(const Vec2f &pos)
{
    auto m = s_mechs.activateNext();
    if (m != nullptr) {
        m->setup(pos);
    }
}

bool Enemy::updateMech(EnemyMech *mech, float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal});
    static uint16_t bulletMask = Helpers::getMask({Targets::EnemyTarget, Targets::GroundTarget});

    if (!Camera::inActiveZone({mech->m_rect.centerX(), mech->m_rect.centerY()})) {
        return false;
    }

    float px = Camera::tl_x();
    float py = Camera::tl_y();
    float tx = px + 55;
    float ty = py + 44;

    mech->m_counter++;
    Vec2f dir = {tx - mech->m_rect.centerX(), ty - mech->m_rect.centerY()};
    float len = dir.length();
    if (len > 0) {
        dir = dir / len;
    }
    switch (mech->status) {
    case EnemyMech::Mode::Walking:
        if (len < 10) {
            dir *= -1;
        } else if (len < 20) {
            dir *= 0;
        }
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
            ProjectileManager::create({mech->m_rect.centerX(), mech->m_rect.centerY()}, dir * 50.0f, 2, 3.0)
                    ->setSprite({projectile[0], projectile[1]}, 20.0)
                    ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
            mech->status = EnemyMech::Mode::Walking;
            mech->m_counter = rand() % 40;
        }
        break;
    default:
        break;
    }
    Vec2f pos = CollisionManager::resolveMovement({mech->m_rect.centerX(), mech->m_rect.centerY() + 2}, mech->m_velocity * 0.014f, mask);
    pos.setY(pos.y()-2);
    mech->m_rect.setCenter(pos.x(), pos.y());
    int damage = ProjectileManager::getCollisionDamage(pos, 4, bulletMask);
    mech->m_life -= damage;
    if (mech->m_life <= 0) {
        EffectManager::create(pos, {explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f);
        return false;
    } else {
        if (damage > 0) {
            mech->m_damage_frames = 10;
            EffectManager::create(pos - Vec2f(3.5f, 3.5f), {hit[0], hit[1], hit[2], hit[3], hit[4]}, 20.0f);
        }
    }
    if (mech->m_damage_frames > 0) {
        --mech->m_damage_frames;
    }
    return true;
}

void Enemy::updateMechs(float dt)
{
    EnemyMech * start = s_mechs.objects();
    int i = 0;
    while (i < s_mechs.objectCount()) {
        if (!updateMech(start + i, dt)) {
            s_mechs.deactivate(i);
        } else {
            ++i;
        }
    }
}

void Enemy::drawMech(RenderSystem *renderer, EnemyMech *mech)
{
    auto pos = Camera::worldToScreen({mech->m_rect.centerX(), mech->m_rect.centerY()});
    if (mech->m_damage_frames > 0) {
        renderer->sprite(pos.x()-2, pos.y()-2, enemy_ground[4], enemy_ground[4][2]);
    } else if (mech->status == EnemyMech::Mode::Preparing) {
        int idx = mech->m_counter % 10 < 5 ? 1 : 3;
        renderer->sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    } else {
        const int increment = 7;
        int idx = mech->m_counter % (increment*4);
        idx = idx < increment ? 0 : idx < increment*2 ? 1 : idx < increment*3 ? 2 : 1;
        renderer->sprite(pos.x()-2, pos.y()-2, enemy_ground[idx], enemy_ground[idx][2]);
    }
}

void Enemy::drawMechs(RenderSystem *renderer)
{
    EnemyMech * start = s_mechs.objects();
    for (int i = 0; i < s_mechs.objectCount(); ++i) {
        drawMech(renderer, start + i);
    }
}
