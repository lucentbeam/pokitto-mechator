#ifndef SCREENBUFFER_H
#define SCREENBUFFER_H

#include <stdint.h>

template<uint8_t Width, uint8_t Height>
class ScreenBuffer
{
    uint8_t m_buffer[Width*Height];
public:
    uint8_t * getBuffer() { return m_buffer; }

    void shift(int8_t x, int8_t y);

    void drawTile(int16_t x, int16_t y, const uint8_t* tile);
};

#endif // SCREENBUFFER_H
