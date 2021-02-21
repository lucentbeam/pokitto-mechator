#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <cmath>
#include <algorithm>

#include "core/rendering/rendersystem.h"
#include "core/rendering/tilemap.h"

class Camera {
    int m_x,m_y;
    float m_offset_x, m_offset_y;

    const uint8_t screen_half_x = 55, screen_half_y = 44;

    int m_tile_width, m_tile_height;

public:
    const uint8_t render_width = 20, render_height = 16;

    void update(int center_x, int center_y, int map_width, int map_height, int tile_width, int tile_height) {
        float px = std::clamp<float>(center_x - screen_half_x, 0.0f, map_width * tile_width - screen_half_x * 2.0f);
        float py = std::clamp<float>(center_y - screen_half_y, 0.0f, map_height * tile_height - screen_half_y * 2.0f);
        m_x = int(px / tile_width);
        m_y = int(py / tile_height);
        m_offset_x = -std::fmod(px, float(tile_width));
        m_offset_y = -std::fmod(py, float(tile_height));

        m_tile_width = tile_width;
        m_tile_height = tile_height;
    }

    float tl_x() const { return m_x * m_tile_width - m_offset_x; }
    float tl_y() const { return m_y * m_tile_height - m_offset_y; }
    float offset_x() const { return m_offset_x; }
    float offset_y() const { return m_offset_y; }
};

#endif // CAMERA_H
