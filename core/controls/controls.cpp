#include "controls.h"

bool previousUp = false;
bool previousDown = false;
bool previousLeft = false;
bool previousRight = false;

#ifndef POKITTO_SFML

#include "Pokitto.h"
#include <limits>

const ControlStatus Controls::getStatus(bool normalize_dir)
{
    m_stats.up.update(Pokitto::Core::upBtn());
    m_stats.down.update(Pokitto::Core::downBtn());
    m_stats.left.update(Pokitto::Core::leftBtn());
    m_stats.right.update(Pokitto::Core::rightBtn());

    m_stats.y = 0;
    if (m_stats.up.downCount > 0 && m_stats.down.downCount > 0) { // NOTE: dpad on pokitto can get stuck with both buttons down after quickly alternating
      m_stats.y = m_stats.up.downCount < m_stats.down.downCount ? -1 : 1;
    } else if (m_stats.up.downCount > 0) {
      m_stats.y = -1;
    } else if (m_stats.down.downCount > 0) {
      m_stats.y = 1;
    }

    m_stats.x = 0;
    if (m_stats.right.downCount > 0 && m_stats.left.downCount > 0) {
      m_stats.x = m_stats.right.downCount < m_stats.left.downCount ? 1 : -1;
    } else if (m_stats.right.downCount > 0) {
      m_stats.x = 1;
    } else if (m_stats.left.downCount > 0) {
      m_stats.x = -1;
    }

    if (normalize_dir && fabs(m_stats.x) > std::numeric_limits<float>::epsilon() && fabs(m_stats.y) > std::numeric_limits<float>::epsilon()) {
        m_stats.x *= 0.7071f;
        m_stats.y *= 0.7071f;
    }

    m_stats.a.update(Pokitto::Core::aBtn());
    m_stats.b.update(Pokitto::Core::bBtn());
    m_stats.c.update(Pokitto::Core::cBtn());

    return m_stats;
}

#else

#include "SFML/Window.hpp"

const ControlStatus Controls::getStatus(bool normalize_dir)
{
    m_stats.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? 1 : 0);
    m_stats.y = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? 1 : 0);

    if (normalize_dir && fabs(m_stats.x) > std::numeric_limits<float>::epsilon() && fabs(m_stats.y) > std::numeric_limits<float>::epsilon()) {
        m_stats.x *= 0.7071f;
        m_stats.y *= 0.7071f;
    }

    m_stats.up.update(m_stats.y < 0);
    m_stats.down.update(m_stats.y > 0);
    m_stats.left.update(m_stats.x < 0);
    m_stats.right.update(m_stats.x > 0);

    m_stats.a.update(sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
    m_stats.b.update(sf::Keyboard::isKeyPressed(sf::Keyboard::X));
    m_stats.c.update(sf::Keyboard::isKeyPressed(sf::Keyboard::C));

    return m_stats;
}

#endif


