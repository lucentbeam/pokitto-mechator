#ifndef SCREENBUFFER_H
#define SCREENBUFFER_H

#include <stdint.h>

class ScreenBuffer
{
    static const uint8_t screenwidth = 110, screenheight = 88;

    uint8_t m_buffer[screenwidth * screenheight];
public:
    uint8_t * getBuffer() { return m_buffer; }

    void shift(int8_t x, int8_t y);

    void drawTile(int16_t x, int16_t y, const uint8_t* tile);
};

#endif // SCREENBUFFER_H
