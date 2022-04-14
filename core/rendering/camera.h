#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <cmath>
#include <algorithm>

#include "core//rendersystem.h"
#include "core/utilities/vec.h"
#include "core/utilities/rect.h"
#include "core/utilities/tween.h"

class Camera {
    static const uint8_t screen_half_x = 55, screen_half_y = 44;

    static int s_x,s_y;

    static Rect s_region_bounds;

    static bool s_regions_changed;

    static struct ShakeData {
        Vec2f position = Vec2f(3.56f, 2.83f);
        Vec2f velocity = Vec2f(0.0f, 0.0f);
        float scale, duration = 1.0f, timer = 0.0f;
        Vec2f offset;
    } s_shake;

    static struct MotionData {
        bool active = false;
        Tween motionTween = Tween(1.0f);
        Vec2f start, end;
    } s_motion;
public:

    static void update(int center_x, int center_y);

    static float tl_x() { return s_x; }
    static float draw_x() { return s_x + int(s_shake.offset.x()); }
    static float tl_y() { return s_y; }
    static float draw_y() { return s_y + int(s_shake.offset.x()); }

    static Vec2f center();

    static Vec2f worldToScreen(const Vec2f &pos);

    static bool inActiveZone(const Vec2f &pos);

    static bool inActiveZone(const Vec2f &pos, const Vec2f &bounds);

    static bool inViewingZone(const Vec2f &pos);

    static bool hasMovedRegions() { return s_regions_changed; }

    static void moveTo(const Vec2f &pos, float velocity);

    static bool atMovementDestination();

    static void stopMovement();

    static void shake(float intensity, float duration);
};

#endif // CAMERA_H
