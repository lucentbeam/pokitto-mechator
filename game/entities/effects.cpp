#include "effects.h"
#include "core/rendering/camera.h"
#include "game/sprites.h"

Effect EffectManager::s_effects[20];

uint8_t EffectManager::s_effectCount;

void EffectManager::create(const Vec2f &pos, std::initializer_list<const uint8_t *> frames, float fps, uint8_t delay)
{
    s_effects[s_effectCount].pos = pos;
    s_effects[s_effectCount].sprite = SpriteWrapper(frames, fps);
    s_effects[s_effectCount].lifetime = s_effects[s_effectCount].sprite.countsPerCycle();
    s_effects[s_effectCount].delay = delay;
    s_effectCount++;
}

void EffectManager::createExplosion(const Vec2f &pos, int radius, int count)
{
    Vec2f dir(1, 0);
    float r;
    for(int i = 0; i < count; ++i) {
        dir.rotBy(90 + (rand() % 180));
        r = float(rand() % radius*2/3) + radius/3;
        EffectManager::create(pos + dir * r, {explosion_small[0], explosion_small[1], explosion_small[2], explosion_small[3], explosion_small[4], explosion_small[5], explosion_small[6], explosion_small[7], explosion_small[7], explosion_small[7], explosion_small[7]}, 20.0f, rand() % 40);
    }
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
