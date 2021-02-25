#include "screenbuffer.h"

#include <cstring>
#include <cmath>
#include <algorithm>

void ScreenBuffer::shift(int8_t x, int8_t y)
{
    uint8_t source_x_offset = x < 0 ? 0 : uint8_t(x);
    uint8_t dest_x_offset = x < 0 ? uint8_t(-x) : 0;
    std::memmove(m_buffer + dest_x_offset, m_buffer + source_x_offset, screenwidth * screenheight - std::abs(x));

    uint8_t source_y_offset = y > 0 ? uint8_t(y) * screenwidth : 0;
    uint8_t dest_y_offset = y > 0 ? 0 : uint8_t(-y) * screenwidth;
    uint8_t newHeight = screenheight - std::abs(y);
    std::memmove(m_buffer + dest_y_offset, m_buffer + source_y_offset, newHeight * screenwidth);

}

void ScreenBuffer::drawTile(int16_t x, int16_t y, const uint8_t *tile)
{
    uint8_t w = tile[0];
    uint8_t h = tile[1];
    if (y<-h || y>screenheight) return; //invisible
    if (x<-w || x>screenwidth) return;  //invisible
    int screenh = std::min((int)h, screenheight- y);
    int screenw = std::min((int)w, screenwidth - x);
    int screenx = x;
    int offsetx = 0;
    if (x < 0) {
        screenx = 0;
        offsetx = -x;
        screenw -= offsetx;
    }
    int screeny = y;
    int offsety = 0;
    if (y < 0) {
        screeny = 0;
        offsety = -y;
        screenh = h + y;
    }
    uint8_t* loc = m_buffer + 110 * screeny + screenx;
    uint8_t* tloc = const_cast<uint8_t*>(tile) + 2 + offsetx + offsety * w;
    for (int j = 0; j < screenh; j++) {
      std::memcpy(loc, tloc, screenw);
      loc += screenwidth;
      tloc += w;
    }
}
