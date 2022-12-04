#include "mapmanager.h"


#include <cstring>
#include <cmath>
#include <algorithm>

#include "game/maps/worldtiles.h"
#include "game/sprites.h"
//#include "game/maps/worldmutables.h"
#include "game/enums.h"

#include "core/utilities/fpshelper.h"
#include "core/serialization.h"
#include "game/variables.h"

BackgroundMap MapManager::s_background(jungletiles, island_main, island_main_indices, delta_x_island_main, delta_y_island_main, island_main_mutable_indices, island_main_mutable_index_indices, island_main_current_tiles, island_main_last_mutable_index);
BackgroundMap MapManager::s_island1(jungletiles, island_1, island_1_indices, delta_x_island_1, delta_y_island_1, island_1_mutable_indices, island_1_mutable_index_indices, island_1_current_tiles, island_1_last_mutable_index);
BackgroundMap MapManager::s_island2(jungletiles, island_2, island_2_indices, delta_x_island_2, delta_y_island_2, island_2_mutable_indices, island_2_mutable_index_indices, island_2_current_tiles, island_2_last_mutable_index);
BackgroundMap MapManager::s_island3(jungletiles, island_3, island_3_indices, delta_x_island_3, delta_y_island_3, island_3_mutable_indices, island_3_mutable_index_indices, island_3_current_tiles, island_3_last_mutable_index);

SkyTilemap MapManager::s_foreground(jungletiles_sky, island_main_sky, island_main_sky_indices);

ScreenTileStore MapManager::s_camera_tiles;

uint8_t MapManager::getTileAtPvt(float x, float y)
{
    if (s_island1.contains(x, y)) {
        return s_island1.getTileAt(x, y);
    } else
    if (s_island2.contains(x, y)) {
        return s_island2.getTileAt(x, y);
    } else
    if (s_island3.contains(x, y)) {
        return s_island3.getTileAt(x, y);
    } else
    if (s_background.contains(x, y)) {
        return s_background.getTileAt(x, y);
    }

    int px = std::floor(x / 6);
    int py = std::floor(y / 6);
    int v = (px + py * 103);
    int spacer = 91;
    if (v % 3 == 0) {
        spacer = 59;
    } else if (v % 3 == 1) {
        spacer = 113;
    }
    int tile = v % 5 == 0 ? 52 : 51;
    if (v % spacer == 0) return tile;
    return 19;
}

void MapManager::update()
{
    if (s_camera_tiles.requiresUpdate()) {
        float x               = Camera::tl_x();
        float y               = Camera::tl_y();

        Vec2i delta           = s_camera_tiles.update();
        uint8_t * tile_buffer = s_camera_tiles.getMap();

        int16_t offset = -delta.x() - delta.y() * s_camera_tiles.width();
        if (offset > 0) {
            offset = offset > storeTileCount ? storeTileCount : offset;
            std::memmove(tile_buffer + offset, tile_buffer, storeTileCount - offset);
        } else {
            offset = offset < -storeTileCount ? -storeTileCount : offset;
            std::memmove(tile_buffer, tile_buffer - offset, storeTileCount + offset);
        }
        for(int j = 0; j < s_camera_tiles.height(); ++j) {
            for (int i = 0; i < s_camera_tiles.width(); ++i) {
                if ( (delta.x() < 0 && i < -delta.x()) ||
                     (delta.x() > 0 && i >= (s_camera_tiles.width()-delta.x())) ||
                     (delta.y() < 0 && j < -delta.y()) ||
                     (delta.y() > 0 && j >= (s_camera_tiles.height()-delta.y()))
                     ) {
                    *tile_buffer = getTileAtPvt(x + i * 6, y + j * 6);
                }
                tile_buffer++;
            }
        }
    }
}

void MapManager::rebuildVisibleTiles()
{
    float x               = Camera::tl_x();
    float y               = Camera::tl_y();

    Vec2i delta           = s_camera_tiles.update();
    uint8_t * tile_buffer = s_camera_tiles.getMap();

    for(int j = 0; j < s_camera_tiles.height(); ++j) {
        for (int i = 0; i < s_camera_tiles.width(); ++i) {
            *tile_buffer = getTileAtPvt(x + i * 6, y + j * 6);
            tile_buffer++;
        }
    }
}

void MapManager::draw(bool bg)
{
    if (bg) {
        float x = Camera::draw_x();
        float y = Camera::draw_y();
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

int MapManager::getMapIndex(float x, float y)
{
    int px = (x / 6);
    int py = (y / 6);
    return px + py * 1000; // arbitrary value created in level export script
//    return s_background.getMapIndex(x, y);
}

uint8_t MapManager::getTileAt(float x, float y)
{
    uint8_t t = s_camera_tiles.getTileAt(x, y);
    if (t != 255) return t;
    return getTileAtPvt(x, y);
}

void MapManager::setTileAt(float x, float y, uint8_t override)
{
    if (s_island1.contains(x, y)) {
        s_island1.setTileAt(x, y, override);
    } else if (s_island2.contains(x, y)) {
        s_island2.setTileAt(x, y, override);
    } else if (s_island3.contains(x, y)) {
        s_island3.setTileAt(x, y, override);
    } else if (s_background.contains(x, y)) {
        s_background.setTileAt(x, y, override);
    }
    s_camera_tiles.setTileAt(x, y, override);
}

void MapManager::loadMutables(char *file)
{
    int start = sizeof(GameStorage);
    Serialization::tryGet<uint8_t>(file, island_main_current_tiles, start, sizeof(island_main_current_tiles));
    start += sizeof(island_main_current_tiles);
    Serialization::tryGet<uint8_t>(file, island_1_current_tiles, start, sizeof(island_1_current_tiles));
    start += sizeof(island_1_current_tiles);
    Serialization::tryGet<uint8_t>(file, island_2_current_tiles, start, sizeof(island_2_current_tiles));
    start += sizeof(island_2_current_tiles);
    Serialization::tryGet<uint8_t>(file, island_3_current_tiles, start, sizeof(island_3_current_tiles));
}

void MapManager::dumpMutables(char *file)
{
    Serialization::tryAppend<uint8_t>(file, island_main_current_tiles, sizeof(island_main_current_tiles));
    Serialization::tryAppend<uint8_t>(file, island_1_current_tiles, sizeof(island_1_current_tiles));
    Serialization::tryAppend<uint8_t>(file, island_2_current_tiles, sizeof(island_2_current_tiles));
    Serialization::tryAppend<uint8_t>(file, island_3_current_tiles, sizeof(island_3_current_tiles));
}

void MapManager::resetMutables()
{
    std::memcpy(island_main_current_tiles, island_main_default_tiles, sizeof(island_main_current_tiles));
    std::memcpy(island_1_current_tiles, island_1_default_tiles, sizeof(island_1_current_tiles));
    std::memcpy(island_2_current_tiles, island_2_default_tiles, sizeof(island_2_current_tiles));
    std::memcpy(island_3_current_tiles, island_3_default_tiles, sizeof(island_2_current_tiles));
}

