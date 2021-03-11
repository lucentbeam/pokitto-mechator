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

    s_controls.m_stats.up.update(s_controls.m_stats.y < 0);
    s_controls.m_stats.down.update(s_controls.m_stats.y > 0);
    s_controls.m_stats.left.update(s_controls.m_stats.x < 0);
    s_controls.m_stats.right.update(s_controls.m_stats.x > 0);

    s_controls.m_stats.a.update(sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
    s_controls.m_stats.b.update(sf::Keyboard::isKeyPressed(sf::Keyboard::X));
    s_controls.m_stats.c.update(sf::Keyboard::isKeyPressed(sf::Keyboard::C));
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


