#ifndef PICKUPS_H
#define PICKUPS_H

#include "core/rendersystem.h"
#include "core/utilities/vec.h"
#include "core/rendering/spritewrapper.h"
#include "game/player.h"

class Pickups
{
    static ObjectPool<Pickups, 8> s_temporary;
    static ObjectPool<Pickups, 12> s_special;

    static std::vector<uint16_t> s_acquired_specials; // store map indices of specials that have already been acquired

    Vec2i position{0,0};
    SpriteWrapper m_sprite;
    void (*m_on_collect)(const Vec2i&);
    uint16_t m_lifetime;

    void configure(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&), uint16_t lifetime);

    static void spawnTemporary(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&), uint16_t lifetime);

    static void spawnSpecial(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&));

    static bool mapIndexUnacquired(const Vec2i &pos);

    static void acquireAtIndex(const Vec2i &pos);
public:

    static void spawnDollar(const Vec2f &pos);

    static void spawnHackingKit(const Vec2i &pos);

    static void spawnKeycardA(const Vec2i &pos);
    static void spawnKeycardB(const Vec2i &pos);
    static void spawnKeycardC(const Vec2i &pos);

    static void spawnBlueprint(const Vec2i &pos);

    static void update(float dt);

    static void draw();
};

#endif // PICKUPS_H
