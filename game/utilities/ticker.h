#ifndef TICKER_H
#define TICKER_H

#include <cstdint>

class GenericTicker {
    uint8_t m_count = 0;

public:
    void reset(uint8_t count);
    void update();
    bool ready() const;
};

#endif // TICKER_H
