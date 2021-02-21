#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "game/rendering/camera.h"

#include "game/tilesets.h"

class Player {
    const float speed = 50.0f;

    float m_x = 32.0f, m_y = 32.0f;

    float fx = 1.0f, fy = 0.0f;

public:
    Player(float x, float y, float speed = 50.0f) :
        m_x(x),
        m_y(y),
        speed(speed)
    {

    }

    void update(float dt, ControlStatus controls) {

        if (controls.x != 0 || controls.y != 0) {
            float dx = controls.x - fx;
            float dy = controls.y - fy;
            const float change_rate = 0.05f;
            fx += dx * change_rate;
            fy += dy * change_rate;
            float len = std::sqrt(fx*fx + fy*fy);
            if (len > 0) {
                fx /= len;
                fy /= len;
            }

            m_x += fx * speed * dt;
            m_y += fy * speed * dt;
        }

        m_x = std::clamp<float>(m_x, 0, 30*6); // TODO: move or remove map bounds
        m_y = std::clamp<float>(m_y, 0, 50*6);
    }

    void draw(RenderSystem * renderSystem, const Camera &camera) {
        bool flip = true;
        uint8_t sprite = 0;

        float angle = std::atan2(fy, std::abs(fx)) * 180.0f / 3.1415926f;
        angle = std::fabs(angle - 90.0f + 22.5f);
        sprite = uint8_t(angle / 45.0f);
        if (fx < 0) {
            flip = false;
        }

        renderSystem->sprite(m_x - camera.tl_x() - 6, m_y - camera.tl_y() - 6, jeep[sprite], jeep[0][2], flip);

        renderSystem->pixel(m_x - camera.tl_x() + fx * 12, m_y - camera.tl_y() + fy * 12, 41);
    }

    float x() const { return m_x; }
    float y() const { return m_y; }
};

#endif // PLAYER_H
