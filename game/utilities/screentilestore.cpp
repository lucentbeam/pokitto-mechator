#include "screentilestore.h"

#include "core/rendering/camera.h"

uint8_t ScreenTileStore::getTileAt(float x, float y) const
{
    int px = std::floor(x/6.0f);
    int py = std::floor(y/6.0f);
    px -= topleft.x();
    py -= topleft.y();

    if (px < 0 || px >= storeTilesW || py < 0 || py >= storeTilesH) return 255;

    return tilemap[px + py * storeTilesW];
}

uint8_t ScreenTileStore::getTileAt(const Vec2f &pos) const
{
    return getTileAt(pos.x(), pos.y());
}

void ScreenTileStore::setTileAt(float x, float y, uint8_t t)
{
    int px = std::floor(x/6.0f);
    int py = std::floor(y/6.0f);
    px -= topleft.x();
    py -= topleft.y();

    if (px < 0 || px > storeTilesW || py < 0 || py > storeTilesH) return;

    tilemap[px + py * storeTilesW] = t;
}

int ScreenTileStore::width() const { return storeTilesW; }

int ScreenTileStore::height() const { return storeTilesH; }

bool ScreenTileStore::requiresUpdate() const
{
    return int(std::floor(Camera::tl_x() / 6.0f)) != topleft.x() || int(std::floor(Camera::tl_y() / 6.0f)) != topleft.y();
}

Vec2i ScreenTileStore::update()
{
    Vec2i prev = topleft;
    topleft.setX(std::floor(Camera::tl_x()/6.0f));
    topleft.setY(std::floor(Camera::tl_y()/6.0f));
    return topleft - prev;
}

uint8_t *ScreenTileStore::getMap()
{
    return tilemap;
}
