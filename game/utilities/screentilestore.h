#ifndef SCREENTILESTORE_H
#define SCREENTILESTORE_H

#include <cstdint>

#include "core/utilities/vec.h"

constexpr int storeTilesW = int(110/6) + 2;
constexpr int storeTilesH = int(88/6) + 2;

class ScreenTileStore
{
    uint8_t tilemap[storeTilesH * storeTilesW];

    Vec2i topleft = Vec2i(-24000, -24000); // so that it updates on frame 1 (unless the camera happens to start near there

public:

    void setTileAt(float x, float y, uint8_t t);

    uint8_t getTileAt(float x, float y) const;
    uint8_t getTileAt(const Vec2f &pos) const;

    int width() const;
    int height() const;

    bool requiresUpdate() const;
    void update();

    uint8_t * getMap();
};

#endif // SCREENTILESTORE_H
