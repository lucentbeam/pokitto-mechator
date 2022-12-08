#ifndef PICKUPS_H
#define PICKUPS_H

#include "core/rendersystem.h"
#include "core/utilities/vec.h"
#include "core/rendering/spritewrapper.h"
#include "game/player.h"
#include "game/ui/ui.h"

class Pickups
{
    static ObjectPool<Pickups, 14> s_temporary;
    static ObjectPool<Pickups, 10> s_special;

    Vec2i position{0,0};
    SpriteWrapper m_sprite;
    uint16_t m_lifetime;

    void configure(const Vec2i &pos, SpriteName spr, uint16_t lifetime);

    static void spawnTemporary(const Vec2i &pos, SpriteName spr, uint16_t lifetime);

    static void spawnSpecial(const Vec2i &pos, SpriteName spr);

    static bool mapIndexUnacquired(const Vec2i &pos);

    static void acquireAtIndex(const Vec2i &pos, void(*func)(int8_t), UI::Element id);
public:

    static void spawnDollar(const Vec2f &pos);

    static void spawnCashBox(const Vec2i &pos);

    static void spawnHackingKit(const Vec2i &pos);

    static void spawnKeycardA(const Vec2i &pos);
    static void spawnKeycardB(const Vec2i &pos);
    static void spawnKeycardC(const Vec2i &pos);

    static void spawnBlueprint(const Vec2i &pos);

    static void update(float dt);

    static void draw();

    static void clear();
};

#endif // PICKUPS_H
