#ifndef __SYSTEM
#define __SYSTEM

#include <cstdint>

struct RenderSystem
{
private:
    uint8_t m_clip_width, m_clip_height;
    bool m_clipping = false;

public:
    uint32_t getTimeMs();

    void initialize();

    bool running();

    bool update();

    void clear(uint8_t idx);

    void print(int x, int y, const char * line, uint8_t color, uint8_t size = 1, uint8_t bgcol = 0);

    void setClip(bool clip, uint8_t clip_width = 0, uint8_t clip_height = 0);

    void sprite(int x, int y, const uint8_t * sprite, int transparent_color = -1, bool flip = false);

    void pixel(int x, int y, uint8_t color);

    void drawLine(int x0, int y0, int x1, int y1, uint8_t color);

    void drawRect(int x0, int y0, int w, int h, uint8_t color);

};

#endif
