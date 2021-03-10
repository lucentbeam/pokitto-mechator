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

    static std::vector<uint16_t> s_acquired_specials; // store map indices of specials that have already been acquired

    Vec2f position{0,0};
    SpriteWrapper m_sprite;
    void (*m_on_collect)(const Vec2f&);
    uint16_t m_lifetime;

    void configure(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)(const Vec2f&), uint16_t lifetime);

    static void spawnTemporary(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)(const Vec2f&), uint16_t lifetime);

    static void spawnSpecial(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)(const Vec2f&));

    static bool mapIndexUnacquired(const Vec2f &pos);

    static void acquireAtIndex(const Vec2f &pos);
public:

    static void spawnDollar(const Vec2f &pos);

    static void spawnHackingKit(const Vec2f &pos);

    static void spawnKeycardA(const Vec2f &pos);
    static void spawnKeycardB(const Vec2f &pos);
    static void spawnKeycardC(const Vec2f &pos);

    static void update(float dt);

    static void draw();
};

#endif // PICKUPS_H
