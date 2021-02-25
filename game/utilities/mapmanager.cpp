#include "mapmanager.h"

BackgroundMap MapManager::s_backgrounds[] = {
    BackgroundMap(jungletiles, world)
};

BackgroundMap * MapManager::s_activeBackground;

void MapManager::draw(RenderSystem *renderer)
{
    s_activeBackground->draw(renderer);
}

void MapManager::draw(ScreenBuffer *buffer)
{
    s_activeBackground->drawToBuffer(buffer);
}

void MapManager::setBackground(BGMap idx)
{
    s_activeBackground = s_backgrounds + idx;
    Camera::configure(s_activeBackground->tileWidth(), s_activeBackground->tileHeight());//, s_activeBackground->mapWidth(), s_activeBackground->mapHeight());
}

uint8_t MapManager::getTileAt(float x, float y)
{
    return s_activeBackground->getTileAt(x, y);
}

void MapManager::setTileAt(float x, float y, uint8_t override)
{
    s_activeBackground->setTileAt(x, y, override);
}

