#ifndef _TILESETTER
#define _TILESETTER

#include <cstdint>
#include <map>
#include <vector>

#include "core/rendering/rendersystem.h"
#include "game/rendering/camera.h"
#include "core/rendering/screenbuffer.h"

template<int TileWidth, int TileHeight>
class Tilemap {
  const uint8_t (*m_tiles)[TileWidth*TileHeight+2];

  const uint8_t * m_map;
  const uint8_t m_mapwidth, m_mapheight;

  bool clearBuffer = true;
  int16_t lastCameraX, lastCameraY;

  std::map<int, uint8_t> m_overrides;
  std::vector<Vec2f> m_redraws;
public:
  const uint8_t render_width = 19, render_height = 15;

  Tilemap(const uint8_t tiles[][TileWidth*TileHeight+2], const uint8_t * map);

  void draw(RenderSystem * system);
  void drawToBuffer(ScreenBuffer * buffer);

  uint8_t mapWidth() const { return m_mapwidth; }
  uint8_t mapHeight() const { return m_mapheight; }
  int tileWidth() const { return TileWidth; }
  int tileHeight() const { return TileHeight; }

  uint8_t getTileAt(float x, float y) const;
  uint8_t getTileAt(int index) const;
  void setTileAt(float x, float y, uint8_t override);
};

template<int TileWidth, int TileHeight>
Tilemap<TileWidth, TileHeight>::Tilemap(const uint8_t tiles[][TileWidth*TileHeight+2], const uint8_t *map) :
    m_tiles(tiles),
    m_map(map + 2),
    m_mapwidth(map[0]),
    m_mapheight(map[1])
{

}

template<int TileWidth, int TileHeight>
void Tilemap<TileWidth, TileHeight>::draw(RenderSystem *system)
{
    int16_t x = Camera::tl_x();
    int16_t y = Camera::tl_y();
    float sx = -std::fmod(x, TileWidth);
    float sy = -std::fmod(y, TileHeight);
    x /= TileWidth;
    y /= TileHeight;
    uint16_t x_upper = x + render_width;
    if (x_upper > m_mapwidth) x_upper = m_mapwidth;
    uint16_t y_upper = y + render_height;
    if (y_upper > m_mapheight) y_upper = m_mapheight;
    uint16_t x_lower = x < 0 ? 0 : x;
    uint16_t y_lower = y < 0 ? 0 : y;
    uint16_t idx = x_lower + y_lower * m_mapwidth;
    for(int16_t j = y_lower; j < y_upper; j++) {
        for(int16_t i = x_lower; i < x_upper; i++) {
            uint8_t tile = m_map[idx];
            system->sprite(sx + (i-x) * TileWidth, sy + (j-y) * TileHeight, m_tiles[tile]);
            idx++;
        }
        idx += (m_mapwidth - (x_upper - x_lower));
    }
}

template<int TileWidth, int TileHeight>
void Tilemap<TileWidth, TileHeight>::drawToBuffer(ScreenBuffer *buffer)
{
    if (clearBuffer) {
        int16_t x = Camera::tl_x();
        int16_t y = Camera::tl_y();
        lastCameraX = x;
        lastCameraY = y;
        float sx = -std::fmod(x, TileWidth);
        float sy = -std::fmod(y, TileHeight);
        x /= TileWidth;
        y /= TileHeight;
        uint16_t x_upper = x + render_width;
        if (x_upper > m_mapwidth) x_upper = m_mapwidth;
        uint16_t y_upper = y + render_height;
        if (y_upper > m_mapheight) y_upper = m_mapheight;
        uint16_t x_lower = x < 0 ? 0 : x;
        uint16_t y_lower = y < 0 ? 0 : y;
        uint16_t idx = x_lower + y_lower * m_mapwidth;
        for(int16_t j = y_lower; j < y_upper; j++) {
            for(int16_t i = x_lower; i < x_upper; i++) {
                uint8_t tile = m_map[idx];
                buffer->drawTile(sx + (i-x) * TileWidth, sy + (j - y) * TileHeight, m_tiles[tile]);
                idx++;
            }
            idx += (m_mapwidth - (x_upper - x_lower));
        }
        clearBuffer = false;
    } else {
        int16_t x = Camera::tl_x();
        int16_t y = Camera::tl_y();
        int16_t dx = x - lastCameraX; // positive is right, creates deadspace at the right
        int16_t dy = y - lastCameraY; // positive is down, creates deadspace at bottom


        {
            for(const Vec2f &p : m_redraws) {
                int top = p.y() / TileHeight;
                int left = p.x() / TileWidth;

                int sx = left * TileWidth - x;
                int sy = top * TileHeight - y;

                int idx = left + top * m_mapwidth;

                buffer->drawTile(sx, sy, m_tiles[getTileAt(idx)]);
            }
            m_redraws.clear();
        }
        if (dx == 0 && dy == 0) return;

        buffer->shift(dx, dy);
        lastCameraX = x;
        lastCameraY = y;

        int map_l_tile = x / TileWidth;
        int map_t_tile = y / TileHeight;
        int map_r_tile = (x + 109 - dx) / TileWidth;
        int map_b_tile = (y + 87 - dy) / TileHeight;

        if (dx != 0) {
            int cols_to_draw = dx > 0 ? (x + 109) / TileWidth - map_r_tile : (x - dx) / TileWidth - map_l_tile;
            cols_to_draw++;

            int left = dx > 0 ? map_r_tile : map_l_tile;

            int sx = left * TileWidth - x;
            int sy = map_t_tile * TileHeight - y;

            for(int j = 0; j < render_height; j++) {
                for (int i = 0; i < cols_to_draw; i++) {
                    int idx = left + i + (j + map_t_tile) * m_mapwidth;
                    buffer->drawTile(sx + i * TileWidth, sy + j*TileHeight, m_tiles[getTileAt(idx)]);
                }
            }
        }

        if (dy != 0) {
            int rows_to_draw = dy > 0 ? (y + 87) / TileHeight - map_b_tile : (y - dy) / TileHeight - map_t_tile;
            rows_to_draw++;

            int top = dy > 0 ? map_b_tile : map_t_tile;

            int sx = map_l_tile * TileWidth - x;
            int sy = top * TileHeight - y;

            for(int i = 0; i < render_width; i++) {
                for (int j = 0; j < rows_to_draw; j++) {
                    int idx = map_l_tile + i + (j + top) * m_mapwidth;
                    buffer->drawTile(sx + i * TileWidth, sy + j*TileHeight, m_tiles[getTileAt(idx)]);
                }
            }
        } else {
            // if dx > 0, render lower right tile
            // if dx < 0, render upper left tile
            int top = dx > 0 ? map_b_tile : map_t_tile;
            int left = dx > 0 ? map_r_tile : map_l_tile;

            int sx = left * TileWidth - x;
            int sy = top * TileHeight - y;

            for(int i = 0; i < render_width; i++) {
                int idx = left + i + top * m_mapwidth;
                buffer->drawTile(sx + i * TileWidth, sy, m_tiles[getTileAt(idx)]);
            }
        }
    }
}

template<int TileWidth, int TileHeight>
uint8_t Tilemap<TileWidth, TileHeight>::getTileAt(float x, float y) const
{
    int px = (x / TileWidth);
    int py = (y / TileHeight);
    const uint8_t defaultTileIndex = 19; // deep water; TODO: find a better place for this
    if (x < 0 || y < 0 || px >= m_mapwidth || py >= m_mapheight) return defaultTileIndex;
    return getTileAt(px + py * m_mapwidth);
}

template<int TileWidth, int TileHeight>
uint8_t Tilemap<TileWidth, TileHeight>::getTileAt(int index) const
{
    if (m_overrides.find(index) != m_overrides.end()) {
        return m_overrides.at(index);
    }
    return m_map[index];
}

template<int TileWidth, int TileHeight>
void Tilemap<TileWidth, TileHeight>::setTileAt(float x, float y, uint8_t override)
{
    int px = (x / TileWidth);
    int py = (y / TileHeight);
    if (x < 0 || y < 0 || px >= m_mapwidth || py >= m_mapheight) return;
    m_overrides[px + py * m_mapwidth] = override;
    m_redraws.push_back({x,y});
}

#endif
