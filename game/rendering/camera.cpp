#include "camera.h"

int Camera::s_x;
int Camera::s_y;
float Camera::s_offset_x;
float Camera::s_offset_y;
int Camera::s_tile_width;
int Camera::s_tile_height;
int Camera::s_map_width;
int Camera::s_map_height;
bool Camera::s_clamp = false;

void Camera::configure(int tile_width, int tile_height, int map_width, int map_height)
{
    s_tile_width = tile_width;
    s_tile_height = tile_height;
    s_map_width = map_width;
    s_map_height = map_height;
    s_clamp = map_width != 0 && map_height != 0;
}

void Camera::update(int center_x, int center_y) {
    float px = center_x - screen_half_x;
    float py = center_y - screen_half_y;
    if (s_clamp) {
        px = std::clamp<float>(px, 0.0f, s_map_width * s_tile_width - screen_half_x * 2.0f);
        py = std::clamp<float>(py, 0.0f, s_map_height * s_tile_height - screen_half_y * 2.0f);
    }
    s_x = int(px / s_tile_width);
    s_y = int(py / s_tile_height);
    s_offset_x = -std::fmod(px, float(s_tile_width));
    s_offset_y = -std::fmod(py, float(s_tile_height));
}

Vec2f Camera::worldToScreen(const Vec2f &pos)
{
    return pos - Vec2f(tl_x(), tl_y());
}
