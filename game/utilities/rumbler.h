#ifndef RUMBLER_H
#define RUMBLER_H

#include <cstdint>

struct Rumbler {
    uint16_t m_counter;

public:
    enum Intensity { None, Slight, Vigorous } intensity = Intensity::None;

    void update() { m_counter++; }

    int offset(float scale) {
        int modulo = m_counter % 90;
        switch (intensity) {
        case Intensity::Slight:
            return modulo < 3 ? scale : (modulo < 42 && modulo > 39) ? -scale : 0;
        case Intensity::Vigorous:
            modulo = m_counter % 10;
            return modulo < 2 ? scale : 0;
        default:
            return 0;
        }
    }
};

#endif // RUMBLER_H
