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

public:

    static void update(int center_x, int center_y);

    static float tl_x() { return s_x; }
    static float tl_y() { return s_y; }

    static Vec2f worldToScreen(const Vec2f &pos);
};

#endif // CAMERA_H
