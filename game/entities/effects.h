#ifndef EFFECTS_H
#define EFFECTS_H

#include "core/rendersystem.h"
#include "core/rendering/spritewrapper.h"
#include "core/utilities/vec.h"
#include "core/utilities/objectpool.h"

const int maxEffectCount = 18;

class EffectManager;

class Effect
{
    SpriteWrapper sprite;
    Vec2f pos;
    int16_t lifetime;
    uint8_t delay;

    friend EffectManager;
public:
    Effect() : pos({0,0}) {}
};

class EffectManager
{
    static ObjectPool<Effect, maxEffectCount> s_effects;

    static void create(const Vec2f &pos, SpriteName spr, float delay = 0);
public:
    static void createSmallExplosion(const Vec2f &pos, int delay);

    static void createExplosion(const Vec2f &pos, int radius, int count);

    static void createExplosionBig(const Vec2f &pos);

    static void createHit(const Vec2f &pos);

    static void createSmoke(const Vec2f &pos);

    static void update(float dt);

    static void draw();
};

#endif // EFFECTS_H
