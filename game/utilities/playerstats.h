#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include <cstdint>
#include <algorithm>

class Statistic {
    int8_t m_current;

    const int8_t m_max;

public:
    Statistic(int8_t max) : m_current(max), m_max(max) {}
    Statistic(int8_t value, int8_t max) : m_current(value), m_max(max) {}

    void change(int8_t delta) { m_current = std::clamp<int8_t>(m_current + delta, 0, m_max); }
    void increment() { change(1); }
    void decrement() { change(-1); }

    int8_t value() const { return m_current; }
    int8_t max() const { return m_max; }
};

#endif // PLAYERSTATS_H
