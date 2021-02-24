#include "screenbuffer.h"

#include <cstring>
#include <cmath>
#include <algorithm>

template<uint8_t Width, uint8_t Height>
void ScreenBuffer<Width,Height>::shift(int8_t x, int8_t y)
{
    uint8_t source_x_offset = x > 0 ? 0 : uint8_t(-x);
    uint8_t dest_x_offset = x > 0 ? uint8_t(x) : 0;
    std::memmove(m_buffer + dest_x_offset, m_buffer + source_x_offset, Width * Height - std::abs(x));

    uint8_t source_y_offset = y > 0 ? uint8_t(y) : 0;
    uint8_t dest_y_offset = y > 0 ? 0 : uint8_t(-y);
    uint8_t newHeight = Height - std::abs(y);
    std::memmove(m_buffer + dest_y_offset, m_buffer + source_y_offset, newHeight * Width);

}

template<uint8_t Width, uint8_t Height>
void ScreenBuffer<Width,Height>::drawTile(int16_t x, int16_t y, const uint8_t *tile)
{
    uint8_t w = tile[0];
    uint8_t h = tile[1];
    if (y<-h || y>Height) return; //invisible
    if (x<-w || x>Width) return;  //invisible
    int screenh = std::min((int)h, Height- y);
    int screenw = std::min((int)w, Width - x);
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
    }
    uint8_t* loc = m_buffer + 110 * screeny + screenx;
    uint8_t* tloc = const_cast<uint8_t*>(tile) + offsetx;
    for (int j = 0; j < screenh; j++) {
      std::memcpy(loc, tloc, screenw);
      loc += 110;
      tloc += w;
    }
}
