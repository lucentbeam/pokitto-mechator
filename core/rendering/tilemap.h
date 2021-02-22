#ifndef _TILESETTER
#define _TILESETTER

#include <cstdint>

#include "core/rendering/rendersystem.h"
#include "game/rendering/camera.h"

template<int TileWidth, int TileHeight>
class Tilemap {
  const uint8_t (*m_tiles)[TileWidth*TileHeight+2];

  const uint8_t * m_map;
  const uint8_t m_mapwidth, m_mapheight;

public:
  const uint8_t render_width = 20, render_height = 16;

  Tilemap(const uint8_t tiles[][TileWidth*TileHeight+2], const uint8_t * map);

  void draw(RenderSystem * system);

  uint8_t mapWidth() const { return m_mapwidth; }
  uint8_t mapHeight() const { return m_mapheight; }
  int tileWidth() const { return TileWidth; }
  int tileHeight() const { return TileHeight; }

  uint8_t getTileAt(float x, float y);
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
    int16_t x = Camera::tl_x() / 6;
    int16_t y = Camera::tl_y() / 6;
    float sx = Camera::offset_x();
    float sy = Camera::offset_y();
    uint16_t x_upper = x + render_width;
    if (x_upper > m_mapwidth) x_upper = m_mapwidth;
    uint16_t y_upper = y + render_height;
    if (y_upper > m_mapheight) y_upper = m_mapheight;
    for(int16_t j = y; j < y_upper; j++) {
        if (j < 0) continue;
        for(int16_t i = x; i < x_upper; i++) {
            if (i < 0) continue;
            uint16_t idx = i + j * m_mapwidth;
            uint8_t tile = m_map[idx];
            system->sprite(sx + (i-x) * TileWidth, sy + (j-y) * TileHeight, m_tiles[tile]);
        }
    }
}

template<int TileWidth, int TileHeight>
uint8_t Tilemap<TileWidth, TileHeight>::getTileAt(float x, float y)
{
    if (x < 0 || y < 0) return 0;
    int px = (x / TileWidth);
    int py = (y / TileHeight);
    if (px >= m_mapwidth || py >= m_mapheight) return 0;
    return m_map[px + py * m_mapwidth];
}

#endif
