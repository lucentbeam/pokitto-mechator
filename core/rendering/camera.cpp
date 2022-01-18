#include "camera.h"

const int regionSize = 6 * 14; // 6 pixels per tile, 14 tiles per region

int Camera::s_x;
int Camera::s_y;

Rect Camera::s_region_bounds(0, 0, regionSize * 3, regionSize * 3);
bool Camera::s_regions_changed = true;

Camera::MotionData Camera::s_motion;

void Camera::update(int center_x, int center_y) {
    if (s_motion.active) {
        s_x = s_motion.motionTween.getInterpolationInt(s_motion.start.x(), s_motion.end.x());
        s_y = s_motion.motionTween.getInterpolationInt(s_motion.start.y(), s_motion.end.y());
    } else {
        s_x = center_x - screen_half_x;
        s_y = center_y - screen_half_y;
        int nextx = std::floor(center_x/regionSize) * regionSize + regionSize / 2;
        int nexty = std::floor(center_y/regionSize) * regionSize + regionSize / 2;
        s_regions_changed = nextx != s_region_bounds.centerX() || nexty != s_region_bounds.centerY();
        if (s_regions_changed) {
            s_region_bounds.setCenter(nextx, nexty);
        }
    }
}

Vec2f Camera::center()
{
    return Vec2f(s_x, s_y) + Vec2f(55, 44);
}

Vec2f Camera::worldToScreen(const Vec2f &pos)
{
    return pos - Vec2f(s_x, s_y);
}

bool Camera::inActiveZone(const Vec2f &pos)
{
    return s_region_bounds.contains(pos.x(), pos.y());
}

bool Camera::inViewingZone(const Vec2f &pos)
{
    Vec2f sc = worldToScreen(pos);
    return (sc.x() > -10 && sc.x() < 120 && sc.y() > -10 && sc.y() < 98);
}

void Camera::moveTo(const Vec2f &pos, float velocity)
{
    s_motion.start = Vec2f(s_x, s_y);
    s_motion.end = pos - Vec2f(55, 44);
    s_motion.active = true;
    s_motion.motionTween = Tween(Tween::OutQuad, (s_motion.end - s_motion.start).length() / velocity);
}

bool Camera::atMovementDestination()
{
    return s_motion.motionTween.done();
}

void Camera::stopMovement()
{
    s_motion.active = false;
}
