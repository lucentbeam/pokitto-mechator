#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <cmath>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "game/rendering/camera.h"

#include "game/tilesets.h"

class Steering {
    float m_x, m_y;
    float m_fx = 1.0f, m_fy = 0.0f;
    const float m_speed;
    const float m_cornering;
    const float m_friction;
    bool m_moving;
    float m_interia;
public:
    Steering(float x, float y, float speed, float cornering, float friction = 1.0f) : m_x(x), m_y(y), m_speed(speed), m_cornering(cornering), m_friction(friction) {}

    void update(float dt, float x, float y) {
        m_moving = false;
        if (std::fabs(x) > 0.01f || std::fabs(y) > 0.01f) {
            m_moving = true;
            float dx = x - m_fx;
            float dy = y - m_fy;
            m_fx += dx * m_cornering;
            m_fy += dy * m_cornering;
            float len = std::sqrt(m_fx*m_fx + m_fy*m_fy);
            if (len > 0) {
                m_fx /= len;
                m_fy /= len;
            }
            m_interia = m_speed;
        } else {
            m_interia *= (1.0f - m_friction);
        }
        m_x += m_fx * m_interia * dt;
        m_y += m_fy * m_interia * dt;
    }

    void copyPosition(const Steering &other) {
        m_x = other.m_x;
        m_y = other.m_y;
    }

    bool moving() const { return m_moving; }
    float x() const { return m_x; }
    float y() const { return m_y; }
    float fx() const { return m_fx; }
    float fy() const { return m_fy; }
    uint8_t rotation_frame() const {
        float angle = std::atan2(m_fy, std::abs(m_fx)) * 180.0f / 3.1415926f;
        angle = angle + 90.0f + 11.25f;
        return uint8_t(angle / 180.0f * 8.0f);
    }
};

class Player {
    Controls m_controller;

    Steering m_soldier;
    Steering m_jeep;

    bool m_dismounted = true;

public:
    Player(float x, float y) :
        m_soldier(x, y, 20.0f, 1.0f),
        m_jeep(x, y - 12, 50.0f, 0.1f,0.1f)
    {

    }

    void update(float dt) {
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

    void draw(RenderSystem * renderSystem, const Camera &camera) {
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

    float x() const { return m_dismounted ? m_soldier.x() : m_jeep.x(); }
    float y() const { return m_dismounted ? m_soldier.y() : m_jeep.y(); }
};

#endif // PLAYER_H
