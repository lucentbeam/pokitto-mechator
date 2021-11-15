#ifndef __SYSTEM
#define __SYSTEM

#include <cstdint>
#include "game/palette.h"
//const uint16_t palette[] = {
//0,59847,7662,19676,65110,10501,64323,38605,36511,14760,62626,52973,16711,25260,65029,12711,27118,37741,31112,12905,37621,44207,39463,21484,44094,27244,52039,38224,60797,31633,58505,46546,29162,40278,64970,2796,41549,50938,18916,3153,52015,65535,25382,3443,60435,26916,42312,14103,33003,45510,55049,36828,49450,60006,65520,12682,62063,62441,4809,19022,62480,43270,9356,19254,64818,
//};

 const uint8_t shading[] =
 {
    -0, 1, 1, 2, 3,
     4, 6, 6, 7, 8,
     9, 11, 11, 12, 13,
     11, 15, 16, 17, 18,
     1, 20, 21, 22, 23,
     1, 25, 26, 27, 28,
     25, 30, 31, 32, 33,
     1, 35, 36, 37, 38,
     36, 40, 41, 42, 43,
     1, 45, 46, 47, 48,
     1, 50, 51, 52, 53,
     51, 55, 56, 57, 50,
     59, 60, 61, 62, 63
 };

const uint8_t backgroundColor = 40;

struct RenderSystem
{
private:
    static uint8_t s_clip_width, s_clip_height;
    static bool s_clipping;

public:
    static bool s_slim_font;

    static void initialize();

    static uint32_t getTimeMs();

    static bool running();

    static bool update();

    static void clear(uint8_t idx);

    static void print(int x, int y, const char * line, uint8_t color, uint8_t size = 1, uint8_t bgcol = backgroundColor);

    static int getLineLength(const char * line, int fontSize = 1);

    static void setClip(bool clip, uint8_t clip_width = 0, uint8_t clip_height = 0);

    static void setFontSlim(bool slim);

    static void sprite(int x, int y, const uint8_t * sprite, int transparent_color = -1, bool flip = false);

    static void sprite(int x, int y, const uint8_t * sprite, int transparent_color, int target_color, int replacement_color);

    static void spriteWrapped(int x, int y, const uint8_t * sprite, int transparent_color);

    static void drawCenterScaled(int xc, int yc, const uint8_t * sprite, float xscale, int transparent_color = -1, float yscale = -1.0f);

    static void pixel(int x, int y, uint8_t color);

    static void drawLine(int x0, int y0, int x1, int y1, uint8_t color);

    static void drawRect(int x0, int y0, int w, int h, uint8_t color);

    static void drawCircle(int x, int y, int r, int color);

    static void drawShadow(int x, int y, const uint8_t * sprite, int transparent_color, bool flip = false);

    static void drawBuffer(uint8_t * buffer);

    static void shiftScreen(int x, int y = 0);

    static uint8_t * getBuffer();
};

#endif
