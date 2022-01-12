#ifndef EFFECTS_H
#define EFFECTS_H

#include "core/rendersystem.h"
#include "core/rendering/spritewrapper.h"
#include "core/utilities/vec.h"

const int maxEffectCount = 24;

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
    static Effect s_effects[maxEffectCount];

    static uint8_t s_effectCount;

    static void create(const Vec2f &pos, const uint8_t* frame_start, int framecount, float fps, float delay = 0);
public:


    static void createExplosion(const Vec2f &pos, int radius, int count);

    static void createExplosionBig(const Vec2f &pos);

    static void createHit(const Vec2f &pos);

    static void update(float dt);

    static void draw();
};

#endif // EFFECTS_H
