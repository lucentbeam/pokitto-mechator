#ifndef POIS_H
#define POIS_H

#include "game/player.h"

class POIs
{
    static ObjectPool<POIs, 6> s_pois;

    static std::vector<uint16_t> s_activated;

    static POIs * s_current_active_poi;

    Vec2f m_position{0,0};

    Vec2f m_jeep_loc{0,0};
    Vec2f m_boat_loc{0,0};
    Vec2f m_heli_loc{0,0};

    // only used for keyed doors
    uint16_t m_top, m_left;
    uint8_t m_width, m_height;
    POIType m_door_type;

    SpriteWrapper m_sprite;

    void configure(const Vec2f &pos, std::initializer_list<const uint8_t*> spriteFrames, float spriteFPS);

    static bool mapIndexUnopened(const Vec2f &pos);

    static void openAtIndex(const Vec2f &pos);

public:

    static void spawnDoor(const Vec2f &pos, uint16_t left, uint16_t top, uint8_t width, uint8_t height, POIType door);

    static void spawnShop(const Vec2f &pos, const Vec2f &jeep_loc, const Vec2f &boat_loc, const Vec2f &heli_loc);

    static void unlockCurrent();

    static void update(float dt);

    static void draw();

    static Vec2f pos(PlayerMode mode = PlayerMode::SoldierMode);
};

template <int x1, int y1, int x2, int y2, int x3, int y3>
void spawnShop(const Vec2f &loc) {
    POIs::spawnShop(loc,Vec2f(x1*6,y1*6),Vec2f(x2*6,y2*6),Vec2f(x3*6,y3*6));
}

template <int l, int t, int w, int h>
void spawnDoorA(const Vec2f &loc) {
    POIs::spawnDoor(loc, l*6, t*6, w*6, h*6, POIType::DoorA);
}

template <int l, int t, int w, int h>
void spawnDoorB(const Vec2f &loc) {
    POIs::spawnDoor(loc, l*6, t*6, w*6, h*6, POIType::DoorB);
}

template <int l, int t, int w, int h>
void spawnDoorC(const Vec2f &loc) {
    POIs::spawnDoor(loc, l*6, t*6, w*6, h*6, POIType::DoorC);
}

template <int l, int t, int w, int h>
void spawnDoorNone(const Vec2f &loc) {
    POIs::spawnDoor(loc, l*6, t*6, w*6, h*6, POIType::DoorNone);
}

#endif // POIS_H
