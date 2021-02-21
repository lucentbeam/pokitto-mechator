#ifndef _TILESETTER
#define _TILESETTER

#include <cstdint>

#include "rendersystem.h"

template<int TileWidth, int TileHeight>
class Tilemap {
  const uint8_t (*m_tiles)[TileWidth*TileHeight+2];

  const uint8_t * m_map;
  const uint8_t m_mapwidth, m_mapheight;

public:
  Tilemap(const uint8_t tiles[][TileWidth*TileHeight+2], const uint8_t * map);

  void draw(RenderSystem * system, int16_t x, int16_t y, uint8_t w, uint8_t h, int8_t sx, int8_t sy);

  uint8_t mapWidth() const { return m_mapwidth; }
  uint8_t mapHeight() const { return m_mapheight; }
  int tileWidth() const { return TileWidth; }
  int tileHeight() const { return TileHeight; }
};

template<int TileWidth, int TileHeight>
Tilemap<TileWidth, TileHeight>::Tilemap(const uint8_t tiles[][TileWidth*TileHeight+2], const uint8_t * map) :
    m_tiles(tiles),
    m_map(map + 2),
    m_mapwidth(map[0]),
    m_mapheight(map[1])
{

}

template<int TileWidth, int TileHeight>
void Tilemap<TileWidth, TileHeight>::draw(RenderSystem *system, int16_t x, int16_t y, uint8_t w, uint8_t h, int8_t sx, int8_t sy)
{
    uint16_t x_upper = x + w;
    if (x_upper > m_mapwidth) x_upper = m_mapwidth;
    uint16_t y_upper = y + h;
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

#endif
