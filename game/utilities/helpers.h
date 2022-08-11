#ifndef HELPERS_H
#define HELPERS_H

#include <cstring>
#include <cstdint>
#include <initializer_list>

#include "core//rendersystem.h"

namespace Helpers {
    uint16_t getMask(std::initializer_list<uint8_t> collisions);

    void drawNotchedRect(int x, int y, uint8_t w, uint8_t h, uint8_t color);

    void printHorizontallyCentered(int x, int y, const char * text, uint8_t color);

    void drawRLE(int x, int y, const uint8_t * sprite, int transparent = 0, int frame = -1, uint8_t * buffer = nullptr);
};

#endif // HELPERS_H
