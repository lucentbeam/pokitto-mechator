#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include <cstdint>
#include <algorithm>

class Statistic {
    uint8_t m_current;

    const uint8_t m_max;

public:
    Statistic(uint8_t max) : m_current(max), m_max(max) {}
    Statistic(uint8_t value, uint8_t max) : m_current(value), m_max(max) {}

    void change(int8_t delta) { m_current = std::clamp<uint8_t>(m_current + delta, 0, m_max); }
    void increment() { change(1); }
    void decrement() { change(-1); }

    uint8_t value() const { return m_current; }
    uint8_t max() const { return m_max; }
};

struct PlayerStats
{
    Statistic health_jeep{12};

    Statistic health_soldier{8};
};

#endif // PLAYERSTATS_H
