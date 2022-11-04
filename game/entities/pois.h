#ifndef POIS_H
#define POIS_H

#include "game/player.h"

class POIs
{
    static ObjectPool<POIs, 6> s_pois;

    static POIs * s_current_active_poi;

    static bool s_disable_shops;

    Vec2f m_position{0,0};

    Vec2f m_jeep_loc{0,0};
    Vec2f m_boat_loc{0,0};
    Vec2f m_heli_loc{0,0};

    // only used for keyed doors
    int16_t m_top, m_left;
    uint8_t m_width, m_height;
    uint8_t m_tile;
    POIType m_door_type;

    SpriteWrapper m_sprite;

    void configure(const Vec2f &pos, SpriteName spr);

    static bool mapIndexUnopened(const Vec2f &pos);

    static void openAtIndex(const Vec2f &pos);

public:

    static void spawnDoor(const Vec2i &pos, int16_t left, int16_t top, uint8_t width, uint8_t height, uint8_t tile, POIType door);

    static void spawnShop(const Vec2i &pos, const Vec2f &jeep_loc, const Vec2f &boat_loc, const Vec2f &heli_loc);

    static void unlockCurrent();

    static void update(float dt);

    static void draw();

    static void setShopsDisabled(bool disable) { s_disable_shops = disable; }

    static Vec2f pos(PlayerMode mode = PlayerMode::SoldierMode);

    static bool canBuild(PlayerMode mode);
};

#endif // POIS_H
