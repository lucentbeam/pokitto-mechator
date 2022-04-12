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
    int16_t x = std::floor(Camera::draw_x()/18.0f) - 1;
    int16_t y = std::floor(Camera::draw_y()/18.0f) - 1;
    int sx = int(x) * 18 - Camera::draw_x();
    int sy = int(y) * 18 - Camera::draw_y();
    uint16_t x_upper = x + render_width + 1;
    if (x_upper > m_mapwidth) x_upper = m_mapwidth;
    uint16_t y_upper = y + render_height + 1;
    if (y_upper > m_mapheight) y_upper = m_mapheight;
    uint16_t x_lower = x < 0 ? 0 : x;
    if (x_lower >= x_upper) return;
    uint16_t y_lower = y < 0 ? 0 : y;
    if (y_lower >= y_upper) return;
    int current_tile;
    for(int16_t j = y_lower; j < y_upper; j++) {
        const uint8_t * tile = j == 0 ? m_map : m_map + m_map_indices[(j-1)];
        current_tile = *tile;
        int counter = *(tile + 1);
        for (int t = 0; t < x_lower; ++t) {
            --counter;
            if (counter <= 0) {
                tile += 2;
                counter = *(tile + 1);
            }
        }
        for(int16_t i = x_lower; i < x_upper; ++i) {
            if (current_tile != uint8_t(-1)) {
                RenderSystem::drawShadow(sx + (i-x) * 18, sy + (j-y) * 18 + 3, m_tiles[*tile], m_tiles[*tile][2]);
                RenderSystem::sprite(sx + (i-x) * 18, sy + (j-y) * 18, m_tiles[*tile], m_tiles[*tile][2]);
            }
            counter--;
            if (counter <= 0) {
                tile += 2;
                current_tile = *tile;
                counter = *(tile + 1);
            }
        }
    }
}
