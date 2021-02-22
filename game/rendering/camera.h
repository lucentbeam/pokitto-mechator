#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <cmath>
#include <algorithm>

#include "core/rendering/rendersystem.h"
#include "game/utilities/vec.h"

class Camera {
    static const uint8_t screen_half_x = 55, screen_half_y = 44;

    static int s_x,s_y;
    static float s_offset_x, s_offset_y;

    static int s_tile_width;
    static int s_tile_height;
    static int s_map_width;
    static int s_map_height;
    static bool s_clamp;

public:

    static void configure(int tile_width, int tile_height, int map_width = 0, int map_height = 0);

    static void update(int center_x, int center_y);

    static float tl_x() { return s_x * s_tile_width - s_offset_x; }
    static float tl_y() { return s_y * s_tile_height - s_offset_y; }
    static float offset_x() { return s_offset_x; }
    static float offset_y() { return s_offset_y; }

    static Vec2f worldToScreen(const Vec2f &pos);
};

#endif // CAMERA_H
