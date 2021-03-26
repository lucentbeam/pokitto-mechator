#include "controls.h"

#include <cmath>

Controls Controls::s_controls;

#ifndef POKITTO_SFML

#include "Pokitto.h"
#include <limits>

void Controls::update() {
    s_controls.m_stats.up.update(Pokitto::Core::upBtn());
    s_controls.m_stats.down.update(Pokitto::Core::downBtn());
    s_controls.m_stats.left.update(Pokitto::Core::leftBtn());
    s_controls.m_stats.right.update(Pokitto::Core::rightBtn());

    s_controls.m_stats.y = 0;
    if (s_controls.m_stats.up.downCount > 0 && s_controls.m_stats.down.downCount > 0) { // NOTE: dpad on pokitto can get stuck with both buttons down after quickly alternating
      s_controls.m_stats.y = s_controls.m_stats.up.downCount < s_controls.m_stats.down.downCount ? -1 : 1;
    } else if (s_controls.m_stats.up.downCount > 0) {
      s_controls.m_stats.y = -1;
    } else if (s_controls.m_stats.down.downCount > 0) {
      s_controls.m_stats.y = 1;
    }

    s_controls.m_stats.x = 0;
    if (s_controls.m_stats.right.downCount > 0 && s_controls.m_stats.left.downCount > 0) {
      s_controls.m_stats.x = s_controls.m_stats.right.downCount < s_controls.m_stats.left.downCount ? 1 : -1;
    } else if (s_controls.m_stats.right.downCount > 0) {
      s_controls.m_stats.x = 1;
    } else if (s_controls.m_stats.left.downCount > 0) {
      s_controls.m_stats.x = -1;
    }

    s_controls.m_stats.a.update(Pokitto::Core::aBtn());
    s_controls.m_stats.b.update(Pokitto::Core::bBtn());
    s_controls.m_stats.c.update(Pokitto::Core::cBtn());
}

const ControlStatus Controls::getStatus(bool normalize_dir)
{
    ControlStatus out = s_controls.m_stats;

    if (normalize_dir && fabs(out.x) > std::numeric_limits<float>::epsilon() && fabs(out.y) > std::numeric_limits<float>::epsilon()) {
        out.x *= 0.7071f;
        out.y *= 0.7071f;
    }

    return out;
}

#else

#include "SFML/Window.hpp"

void Controls::update()
{
    s_controls.m_stats.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? 1 : 0);
    s_controls.m_stats.y = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? 1 : 0);

    bool kb_a = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    bool kb_b = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    bool kb_c = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
    bool ctrl_a = false, ctrl_b = false, ctrl_c = false;

    const int controller = 0;
    if (sf::Joystick::isConnected(controller)) {
        if (sf::Joystick::hasAxis(controller, sf::Joystick::Axis::X) && sf::Joystick::hasAxis(controller, sf::Joystick::Axis::Y)) {
            float x = sf::Joystick::getAxisPosition(controller, sf::Joystick::Axis::X) / 100;
            float y = sf::Joystick::getAxisPosition(controller, sf::Joystick::Axis::Y) / 100;
            if (std::fabs(x) > 0.2f && std::fabs(x) > std::fabs(s_controls.m_stats.x)) {
                s_controls.m_stats.x = x;
            }
            if (std::fabs(y) > 0.2f && std::fabs(y) > std::fabs(s_controls.m_stats.y)) {
                s_controls.m_stats.y = y;
            }
        }
        if (sf::Joystick::hasAxis(controller, sf::Joystick::Axis::PovX) && sf::Joystick::hasAxis(controller, sf::Joystick::Axis::PovY)) {
            float x = sf::Joystick::getAxisPosition(controller, sf::Joystick::Axis::PovX) / 100;
            float y = -sf::Joystick::getAxisPosition(controller, sf::Joystick::Axis::PovY) / 100;
            if (std::fabs(x) > 0.2f && std::fabs(x) > std::fabs(s_controls.m_stats.x)) {
                s_controls.m_stats.x = x;
            }
            if (std::fabs(y) > 0.2f && std::fabs(y) > std::fabs(s_controls.m_stats.y)) {
                s_controls.m_stats.y = y;
            }
        }
        ctrl_a = sf::Joystick::isButtonPressed(controller, 0);
        ctrl_b = sf::Joystick::isButtonPressed(controller, 2);
        ctrl_c = sf::Joystick::isButtonPressed(controller, 7);
    }

    s_controls.m_stats.a.update(kb_a || ctrl_a);
    s_controls.m_stats.b.update(kb_b || ctrl_b);
    s_controls.m_stats.c.update(kb_c || ctrl_c);

    s_controls.m_stats.up.update(s_controls.m_stats.y < 0);
    s_controls.m_stats.down.update(s_controls.m_stats.y > 0);
    s_controls.m_stats.left.update(s_controls.m_stats.x < 0);
    s_controls.m_stats.right.update(s_controls.m_stats.x > 0);
}

const ControlStatus Controls::getStatus(bool normalize_dir)
{
    ControlStatus out = s_controls.m_stats;

    if (normalize_dir && std::fabs(out.x) > std::numeric_limits<float>::epsilon() && std::fabs(out.y) > std::numeric_limits<float>::epsilon()) {
        out.x *= 0.7071f;
        out.y *= 0.7071f;
    }

    return out;
}

#endif


