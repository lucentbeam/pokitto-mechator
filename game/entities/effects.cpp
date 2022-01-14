#include "effects.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"

ObjectPool<Effect, maxEffectCount> EffectManager::s_effects;

void EffectManager::create(const Vec2f &pos, SpriteName spr, float delay)
{
    auto * e = s_effects.activateNext();
    if (e != nullptr) {
        e->pos = pos;
        e->sprite = SpriteWrapper(spr);
        e->lifetime = e->sprite.countsPerCycle();
        e->delay = uint8_t(delay / physicsTimestep);
    }
}

void EffectManager::createSmallExplosion(const Vec2f &pos, int delay)
{
    EffectManager::create(pos - Vec2f(explosion_small[0][0], explosion_small[0][1])/2, ExplosionSmall, float(delay) / 60.0f);
}

void EffectManager::createExplosion(const Vec2f &pos, int radius, int count)
{
    Vec2f dir(1, 0);
    float r;
    for(int i = 0; i < count; ++i) {
        dir.rotBy(90 + (rand() % 180));
        r = float(rand() % std::max(1, radius*2/3)) + radius/3;
        EffectManager::create(pos + dir * r, ExplosionSmall, i == 0 ? 0 : float(rand() % 40)/60.0f);
    }
}

void EffectManager::createExplosionBig(const Vec2f &pos)
{
    EffectManager::create(pos, ExplosionBig);
}

void EffectManager::createHit(const Vec2f &pos)
{
    EffectManager::create(pos, HitSprite);
}

void EffectManager::createSmoke(const Vec2f &pos)
{
    EffectManager::create(pos, SmokeSprite);
}

void EffectManager::update(float dt)
{
    s_effects.iterate([](Effect * e) {
        if (e->delay == 0) {
            --e->lifetime;
            if (e->lifetime < 0) {
                return true;
            }
        } else {
            --e->delay;
        }
        return false;
    });
}

void EffectManager::draw()
{
    s_effects.iterate([](Effect * e){
        if (e->delay > 0) return false;
        Vec2f pos = Camera::worldToScreen(e->pos);
        RenderSystem::sprite(pos.x(), pos.y(), e->sprite.data(), e->sprite.data()[2]);
        return false;
    });
}
