#ifndef POIS_H
#define POIS_H

#include "game/player.h"

class POIs
{
    static ObjectPool<POIs, 6> s_pois;

    static std::vector<uint16_t> s_activated;

    Vec2f position{0,0};
    SpriteWrapper m_sprite;
    void (*m_on_collect)(const Vec2f&);

    void configure(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS, void (*on_collect)(const Vec2f&));

    static bool mapIndexUnopened(const Vec2f &pos);

    static void openAtIndex(const Vec2f &pos);
public:

    static void spawnDoor(const Vec2f &pos, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    static void spawnShop(const Vec2f &pos);

    static void update(float dt);

    static void draw(RenderSystem * renderer);
};

#endif // POIS_H
