#ifndef HELPERS_H
#define HELPERS_H

#include <cstring>
#include <cstdint>
#include <initializer_list>

#include "core//rendersystem.h"
#include "core/utilities/vec.h"

namespace Helpers {
    uint16_t getMask(std::initializer_list<uint8_t> collisions);

    void drawNotchedRect(int x, int y, uint8_t w, uint8_t h, uint8_t color);

    void printHorizontallyCentered(int x, int y, const char * text, uint8_t color);

    void drawRLE(int x, int y, const uint8_t * sprite, int transparent = 0, int frame = -1, uint8_t * buffer = nullptr, int subx = 0, int suby = 0, int subw = 0, int subh = 0 );

    void drawRotatedBox(Vec2f start, Vec2f dir, float length, int threecolors[3]);
};

#endif // HELPERS_H
