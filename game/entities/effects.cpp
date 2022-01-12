#include "effects.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"

ObjectPool<Effect, maxEffectCount> EffectManager::s_effects;

void EffectManager::create(const Vec2f &pos, const uint8_t *frame_start, int framecount, float fps, float delay)
{
    auto * e = s_effects.activateNext();
    if (e != nullptr) {
        e->pos = pos;
        e->sprite = SpriteWrapper(frame_start, framecount, fps);
        e->lifetime = e->sprite.countsPerCycle();
        e->delay = uint8_t(delay / physicsTimestep);
    }
}

void EffectManager::createExplosion(const Vec2f &pos, int radius, int count)
{
    Vec2f dir(1, 0);
    float r;
    for(int i = 0; i < count; ++i) {
        dir.rotBy(90 + (rand() % 180));
        r = float(rand() % std::max(1, radius*2/3)) + radius/3;
        EffectManager::create(pos + dir * r, explosion_small[0], 7, 20.0f, i == 0 ? 0 : float(rand() % 40)/60.0f);
    }
}

void EffectManager::createExplosionBig(const Vec2f &pos)
{
    EffectManager::create(pos, explosion[0], 7, 20.0f);
}

void EffectManager::createHit(const Vec2f &pos)
{
    EffectManager::create(pos, hit[0], 5, 20.0f);
}

void EffectManager::update(float dt)
{
    s_effects.iterate([](Effect * e) {
        if (e->delay == 0) {
            e->sprite.update();
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
