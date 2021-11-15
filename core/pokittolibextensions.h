#ifndef POKITTOLIBEXTENSIONS_H
#define POKITTOLIBEXTENSIONS_H

#include <cstdint>

#ifndef DESKTOP_BUILD

namespace Pokitto {

    class DisplayExtensions {
        static void shift(int16_t x, int16_t y);

    public:

        static void drawClippedBitmap(int x, int y, int w, int h, int clipw, int cliph, const uint8_t* bitmap);

        static void drawTile(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* tile);

        static void spriteWrapped(int x, int y, const uint8_t * sprite, int transparent_color);

        static void drawCenterScaled(int xc, int yc, const uint8_t* sprite, float scale, int transparent_color, float yscale);

        static void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int transparent_color = -1, bool flip = false);

        static void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int transparent_color, int target_color, int replacement_color);

        static void drawShadow(int16_t x, int16_t y, const uint8_t* bitmap, int transparent_color, const uint8_t * shading, bool flip = false);

        static void shadeAll(int steps, const uint8_t *shading);
    };

}

#endif

#endif // POKITTOLIBEXTENSIONS_H
