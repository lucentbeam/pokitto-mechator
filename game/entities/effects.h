#ifndef EFFECTS_H
#define EFFECTS_H

#include "core/rendering/rendersystem.h"
#include "core/rendering/spritewrapper.h"
#include "core/utilities/vec.h"

class EffectManager;

class Effect
{
    SpriteWrapper sprite;
    Vec2f pos;
    int16_t lifetime;

    friend EffectManager;
public:
    Effect() : pos({0,0}) {}
};

class EffectManager
{
    static Effect s_effects[12];

    static uint8_t s_effectCount;

public:

    static void create(const Vec2f &pos, std::initializer_list<const uint8_t*> frames, float fps);

    static void update(float dt);

    static void draw();
};

#endif // EFFECTS_H
