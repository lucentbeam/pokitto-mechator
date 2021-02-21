#ifndef PLAYER_H
#define PLAYER_H

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "game/rendering/camera.h"

#include "game/tilesets.h"
#include "game/physics/steering.h"

class Player {
    Controls m_controller;

    Steering m_soldier;
    Steering m_jeep;

    bool m_dismounted = true;

public:
    Player(float x, float y);

    void update(float dt);

    void draw(RenderSystem * renderSystem, const Camera &camera);

    float x() const;
    float y() const;
};

#endif // PLAYER_H

Player::Player(float x, float y) :
    m_soldier(x, y, 20.0f, 1.0f),
    m_jeep(x, y - 12, 50.0f, 0.1f,0.1f)
{

}

void Player::update(float dt) {
    ControlStatus controls = m_controller.getStatus(true);
    if (m_dismounted) {
        m_soldier.update(dt, controls.x , controls.y);
        m_jeep.update(dt, 0.0f, 0.0f);
    } else {
        m_jeep.update(dt, controls.x, controls.y);
    }
    if (controls.pb) {
        if (m_dismounted) {
            float dx = m_jeep.x() - m_soldier.x();
            dx *= dx;
            if (dx > 36) return;
            float dy = m_jeep.y() - m_soldier.y();
            dy *= dy;
            if (dy > 36) return;
            m_dismounted = false;
        } else {
            m_dismounted = true;
            m_soldier.copyPosition(m_jeep);
        }

    }
}

void Player::draw(RenderSystem *renderSystem, const Camera &camera) {
    // jeep rendering
    renderSystem->sprite(m_jeep.x() - camera.tl_x() - 7, m_jeep.y() - camera.tl_y() - 7, jeep[m_jeep.rotation_frame()], jeep[0][2], m_jeep.fx() > 0);

    if (m_dismounted) {
        // soldier rendering
        uint8_t sprite = m_soldier.fy() < 0 ? 3 : 0;
        static int counter = 0;
        if (m_soldier.moving()) {
            counter++;
            sprite += counter % 30 < 10 ? 0 : counter % 30 < 20 ? 1 : 2;
        } else {
            sprite += 1;
        }
        renderSystem->sprite(m_soldier.x() - camera.tl_x() - 3, m_soldier.y() - camera.tl_y() - 3, soldier[sprite], soldier[0][2], m_soldier.fx() > 0);
    }
}

float Player::x() const { return m_dismounted ? m_soldier.x() : m_jeep.x(); }

float Player::y() const { return m_dismounted ? m_soldier.y() : m_jeep.y(); }
