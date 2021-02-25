#ifndef __SYSTEM
#define __SYSTEM

#include <cstdint>

const uint16_t palette[] = {
0,59847,7662,19676,65110,10501,64323,38605,36511,14760,62626,52973,16711,25260,65029,12711,27118,37741,31112,12905,37621,44207,39463,21484,44094,27244,52039,38224,60797,31633,58505,46546,29162,40278,64970,2796,41549,50938,18916,3153,52015,65535,25382,3443,60435,26916,42312,14103,33003,45510,55049,36828,49450,60006,65520,12682,62063,62441,4809,19022,62480,43270,9356,19254,64818,
};

const uint8_t shading[] =
{
0,61,62,63,64,0,1,2,3,5,6,7,5,9,10,5,12,13,12,15,16,17,18,19,20,12,22,23,24,25,26,27,12,29,30,55,32,33,15,35,36,37,38,39,40,12,42,43,45,45,46,47,48,49,50,5,52,53,55,55,56,61,58,59,60,
};

const uint8_t backgroundColor = 63;

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

    void print(int x, int y, const char * line, uint8_t color, uint8_t size = 1, uint8_t bgcol = backgroundColor);

    void setClip(bool clip, uint8_t clip_width = 0, uint8_t clip_height = 0);

    void sprite(int x, int y, const uint8_t * sprite, int transparent_color = -1, bool flip = false);

    void pixel(int x, int y, uint8_t color);

    void drawLine(int x0, int y0, int x1, int y1, uint8_t color);

    void drawRect(int x0, int y0, int w, int h, uint8_t color);

    void drawBuffer(uint8_t * buffer);

};

#endif
