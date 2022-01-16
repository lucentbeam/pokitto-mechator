#include "skytilemap.h"

SkyTilemap::SkyTilemap(const uint8_t tiles[][18*18+2], const uint8_t *map, const uint16_t * map_indices) :
    m_tiles(tiles),
    m_map(map + 2),
    m_map_indices(map_indices),
    m_mapwidth(map[0]),
    m_mapheight(map[1])
{

}

void SkyTilemap::draw()
{
    int16_t x = Camera::tl_x();
    int16_t y = Camera::tl_y();
    float sx = -std::fmod(x, 18);
    float sy = -std::fmod(y, 18);
    x /= 18;
    y /= 18;
    uint16_t x_upper = x + render_width;
    if (x_upper > m_mapwidth) x_upper = m_mapwidth;
    uint16_t y_upper = y + render_height;
    if (y_upper > m_mapheight) y_upper = m_mapheight;
    uint16_t x_lower = x < 0 ? 0 : x;
    uint16_t y_lower = y < 0 ? 0 : y;
    for(int16_t j = y_lower; j < y_upper; j++) {
        const uint8_t * tile = j == 0 ? m_map : m_map + m_map_indices[(j-1)];
        int counter = *(tile + 1);
        for (int t = 0; t < x_lower; ++t) {
            --counter;
            if (counter <= 0) {
                tile += 2;
                counter = *(tile + 1);
            }
        }
        for(int16_t i = x_lower; i < x_upper; ++i) {
            if (*tile != uint8_t(-1)) {
                RenderSystem::drawShadow(sx + (i-x) * 18, sy + (j-y) * 18 + 3, m_tiles[*tile], m_tiles[*tile][2]);
                RenderSystem::sprite(sx + (i-x) * 18, sy + (j-y) * 18, m_tiles[*tile], m_tiles[*tile][2]);
            }
            counter--;
            if (counter <= 0) {
                tile += 2;
                counter = *(tile + 1);
            }
        }
    }
}
