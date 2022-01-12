#include "effects.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"

Effect EffectManager::s_effects[maxEffectCount];

uint8_t EffectManager::s_effectCount;

void EffectManager::create(const Vec2f &pos, const uint8_t *frame_start, int framecount, float fps, float delay)
{
    if (s_effectCount == maxEffectCount) {
        return;
    }
    s_effects[s_effectCount].pos = pos;
    s_effects[s_effectCount].sprite = SpriteWrapper(frame_start, framecount, fps);
    s_effects[s_effectCount].lifetime = s_effects[s_effectCount].sprite.countsPerCycle();
    s_effects[s_effectCount].delay = uint8_t(delay / physicsTimestep);
    s_effectCount++;
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
    int i = 0;
    while (i < s_effectCount) {
        if (s_effects[i].delay == 0) {
            s_effects[i].sprite.update();
            --s_effects[i].lifetime;
        } else {
            --s_effects[i].delay;
        }
        if (s_effects[i].lifetime < 0) {
            s_effects[i] = s_effects[s_effectCount-1];
            --s_effectCount;
        } else {
            ++i;
        }
    }
}

void EffectManager::draw()
{
    for (int i = 0; i < s_effectCount; ++i) {
        if (s_effects[i].delay > 0) continue;
        Vec2f pos = Camera::worldToScreen(s_effects[i].pos);
        RenderSystem::sprite(pos.x(), pos.y(), s_effects[i].sprite.data(), s_effects[i].sprite.data()[2]);
    }
}
