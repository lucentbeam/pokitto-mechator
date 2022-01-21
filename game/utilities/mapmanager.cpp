#include "mapmanager.h"

#include "game/maps/mechator.h"
#include "game/maps/mechator_sky.h"
#include "game/sprites.h"
#include "game/maps/worldmutables.h"
#include "game/sprites.h"

#include "core/utilities/fpshelper.h"

BackgroundMap MapManager::s_background(jungletiles, world, world_indices, mutable_indices, mutable_index_indices, current_tiles);

SkyTilemap MapManager::s_foreground(jungletiles_sky, sky, sky_indices);

ScreenTileStore MapManager::s_camera_tiles;

void MapManager::update()
{
    if (s_camera_tiles.requiresUpdate()) {
        s_camera_tiles.update();
        float x = Camera::tl_x();
        float y = Camera::tl_y();
        uint8_t * t = s_camera_tiles.getMap();
        for(int j = 0; j < s_camera_tiles.height(); ++j) {
            for (int i = 0; i < s_camera_tiles.width(); ++i) {
                *t = s_background.getTileAt(x + i * 6, y + j * 6);
                t++;
            }
        }
    }
}

void MapManager::draw(bool bg)
{
    if (bg) {
        float x = Camera::tl_x();
        float y = Camera::tl_y();
        int sx = int(std::floor(x/6.0f)) * 6 - x;
        int sy = int(std::floor(y/6.0f)) * 6 - y;

        uint8_t * t = s_camera_tiles.getMap();
        int tile;
        for(int j = 0; j < s_camera_tiles.height(); ++j) {
            for (int i = 0; i < s_camera_tiles.width(); ++i) {
                tile = *t;
                if (tile == 111) {
                    RenderSystem::drawRect2(sx + i * 6, sy + j * 6, 6, 6, 22);
                } else if (tile == 19) {
                    RenderSystem::drawRect2(sx + i * 6, sy + j * 6, 6, 6, 47);
                } else if (tile == 184) {
                    RenderSystem::drawRect2(sx + i * 6, sy + j * 6, 6, 6, 5);
                    RenderSystem::pixel(sx + i * 6, sy + j * 6, 26);
                } else {
                    RenderSystem::sprite(sx + i * 6, sy + j * 6, jungletiles[tile]);
                }
                t++;
            }
        }
    } else {
        s_foreground.draw();
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
    uint8_t t = s_camera_tiles.getTileAt(x, y);
    if (t != 255) return t;
    return s_background.getTileAt(x, y);
}

void MapManager::setTileAt(float x, float y, uint8_t override)
{
    s_background.setTileAt(x, y, override);
    s_camera_tiles.setTileAt(x, y, override);
}

