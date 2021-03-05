#include "mapmanager.h"

#include "game/maps/mechator.h"
#include "game/maps/mechator_sky.h"
#include "game/tilesets.h"

BackgroundMap MapManager::s_background(jungletiles, world);

SkyTilemap MapManager::s_foreground(jungletiles_sky, sky);

void MapManager::draw(RenderSystem *renderer, bool bg)
{
    if (bg) {
        s_background.draw(renderer);
    } else {
        s_foreground.draw(renderer);
    }
}

void MapManager::draw(ScreenBuffer *buffer)
{
    s_background.drawToBuffer(buffer);
}

uint16_t MapManager::getMapIndex(float x, float y)
{
    return s_background.getMapIndex(x, y);
}

uint8_t MapManager::getTileAt(float x, float y)
{
    return s_background.getTileAt(x, y);
}

void MapManager::setTileAt(float x, float y, uint8_t override)
{
    s_background.setTileAt(x, y, override);
}

void MapManager::clearOverrideAt(float x, float y)
{
    s_background.clearOverrideAt(x, y);
}

