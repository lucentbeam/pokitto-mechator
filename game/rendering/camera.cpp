#include "camera.h"

const int regionSize = 6 * 14; // 6 pixels per tile, 14 tiles per region

int Camera::s_x;
int Camera::s_y;

Rect Camera::s_region_bounds(0, 0, regionSize * 2, regionSize * 2);
bool Camera::s_regions_changed = true;

void Camera::update(int center_x, int center_y) {
    s_x = center_x - screen_half_x;
    s_y = center_y - screen_half_y;
    int nextx = std::floor(center_x/regionSize) * regionSize + regionSize / 2;
    int nexty = std::floor(center_y/regionSize) * regionSize + regionSize / 2;
    s_regions_changed = nextx != s_region_bounds.centerX() || nexty != s_region_bounds.centerY();
    if (s_regions_changed) {
        s_region_bounds.setCenter(nextx, nexty);
    }
}

Vec2f Camera::worldToScreen(const Vec2f &pos)
{
    return pos - Vec2f(s_x, s_y);
}

bool Camera::inActiveZone(const Vec2f &pos)
{
    return s_region_bounds.contains(pos.x(), pos.y());
}
