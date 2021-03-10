#include "effects.h"
#include "game/rendering/camera.h"

Effect EffectManager::s_effects[12];

uint8_t EffectManager::s_effectCount;

void EffectManager::create(const Vec2f &pos, std::initializer_list<const uint8_t *> frames, float fps)
{
    s_effects[s_effectCount].pos = pos;
    s_effects[s_effectCount].sprite = SpriteWrapper(frames, fps);
    s_effects[s_effectCount].lifetime = s_effects[s_effectCount].sprite.countsPerCycle();
    s_effectCount++;
}

void EffectManager::update(float dt)
{
    int i = 0;
    while (i < s_effectCount) {
        s_effects[i].sprite.update();
        --s_effects[i].lifetime;
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
        Vec2f pos = Camera::worldToScreen(s_effects[i].pos);
        RenderSystem::sprite(pos.x(), pos.y(), s_effects[i].sprite.data(), s_effects[i].sprite.data()[2]);
    }
}
