#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <cmath>
#include <algorithm>

#include "core//rendersystem.h"
#include "core/utilities/vec.h"
#include "core/utilities/rect.h"

class Camera {
    static const uint8_t screen_half_x = 55, screen_half_y = 44;

    static int s_x,s_y;

    static Rect s_region_bounds;

    static bool s_regions_changed;

public:

    static void update(int center_x, int center_y);

    static float tl_x() { return s_x; }
    static float tl_y() { return s_y; }

    static Vec2f center();

    static Vec2f worldToScreen(const Vec2f &pos);

    static bool inActiveZone(const Vec2f &pos);

    static bool inViewingZone(const Vec2f &pos);

    static bool hasMovedRegions() { return s_regions_changed; }
};

#endif // CAMERA_H
