#ifndef POKITTOLIBEXTENSIONS_H
#define POKITTOLIBEXTENSIONS_H

#include <cstdint>

#ifndef POKITTO_SFML

namespace Pokitto {

    class DisplayExtensions {
        static void shift(int16_t x, int16_t y);

    public:

        static void drawClippedBitmap(int x, int y, int w, int h, int clipw, int cliph, const uint8_t* bitmap);

        static void drawTile(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* tile);

        static void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int transparent_color = -1, bool flip = false);
    };

}

#endif

#endif // POKITTOLIBEXTENSIONS_H
