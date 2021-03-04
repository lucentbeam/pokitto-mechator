#ifndef PICKUPS_H
#define PICKUPS_H

#include "core/rendering/rendersystem.h"
#include "game/utilities/vec.h"
#include "game/rendering/spritewrapper.h"
#include "game/player.h"

class Pickups
{
    static ObjectPool<Pickups, 10> s_temporary;
    static ObjectPool<Pickups, 4> s_special;

    Vec2f position{0,0};
    SpriteWrapper m_sprite;
    void (*m_on_collect)();
    uint16_t m_lifetime;

    void configure(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)(), uint16_t lifetime);

    static void spawnTemporary(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)(), uint16_t lifetime);

    static void spawnSpecial(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)());

public:

    static void spawnDollar(const Vec2f &pos);

    static void update(float dt);

    static void draw(RenderSystem * renderer);
};

#endif // PICKUPS_H
