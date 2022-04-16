#include "camera.h"

#include "game/constants.h"

const int regionSize = 6 * 14; // 6 pixels per tile, 14 tiles per region

int Camera::s_x;
int Camera::s_y;

Rect Camera::s_region_bounds(0, 0, regionSize * 3, regionSize * 3);
int Camera::s_region_dx = 0;
int Camera::s_region_dy = 0;

Camera::ShakeData Camera::s_shake;

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
        s_region_dx = nextx - s_region_bounds.centerX();
        s_region_dy = nexty - s_region_bounds.centerY();
        if (hasMovedRegions()) {
            s_region_bounds.setCenter(nextx, nexty);
        }
    }
    s_shake.timer -= physicsTimestep;
    float current_scale = s_shake.timer / s_shake.duration * s_shake.scale * 2.0f;
    if (current_scale < 0) current_scale = 0;
    else if (current_scale > 0.25f) current_scale = 1.0f;
    else current_scale = current_scale * 4.0f;

    for(int i = 0; i < 6; ++i) {
        s_shake.position += s_shake.velocity * physicsTimestep;
        Vec2f acc = s_shake.position * s_shake.position.abs() * -1.0f;
        s_shake.velocity += acc * physicsTimestep * 10.0f;
    }

    s_shake.offset = s_shake.position * current_scale;
}

Vec2f Camera::center()
{
    return Vec2f(s_x, s_y) + Vec2f(55, 44);
}

Vec2f Camera::worldToScreen(const Vec2f &pos)
{
    return pos - Vec2f(s_x, s_y) + Vec2i(s_shake.offset.x(), s_shake.offset.y());
}

bool Camera::inActiveZone(const Vec2f &pos)
{
    return s_region_bounds.contains(pos.x(), pos.y());
}

bool Camera::inActiveZone(const Vec2f &pos, const Vec2f &bounds)
{
    Rect r = s_region_bounds;
    r.grow(bounds.x(), bounds.y());
    return r.contains(pos.x(), pos.y());
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

void Camera::shake(float intensity, float duration)
{
    s_shake.scale = intensity;
    s_shake.duration = duration;
    s_shake.timer = duration;
}

int Camera::regionWidth()
{
    return regionSize;
}
