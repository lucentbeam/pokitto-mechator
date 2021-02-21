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

public:
    Player(float x, float y, float speed = 50.0f) :
        m_x(x),
        m_y(y),
        speed(speed)
    {

    }

    void update(float dt, ControlStatus controls) {
        m_x += controls.x * speed * dt;
        m_y += controls.y * speed * dt;
        m_x = std::clamp<float>(m_x, 0, 30*6); // TODO: move or remove map bounds
        m_y = std::clamp<float>(m_y, 0, 50*6);
    }

    void draw(RenderSystem * renderSystem, const Camera &camera) {
        renderSystem->sprite(m_x - camera.tl_x() - 6, m_y - camera.tl_y() - 6, jeep[0], jeep[0][2]);
    }

    float x() const { return m_x; }
    float y() const { return m_y; }
};

#endif // PLAYER_H
