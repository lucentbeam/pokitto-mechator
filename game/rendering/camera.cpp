#include "camera.h"

int Camera::s_x;
int Camera::s_y;

void Camera::update(int center_x, int center_y) {
    s_x = center_x - screen_half_x;
    s_y = center_y - screen_half_y;
}

Vec2f Camera::worldToScreen(const Vec2f &pos)
{
    return pos - Vec2f(s_x, s_y);
}
