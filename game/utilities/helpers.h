#ifndef HELPERS_H
#define HELPERS_H

#include <cstring>
#include <cstdint>
#include <initializer_list>

#include "core//rendersystem.h"

namespace Helpers {
    static uint16_t getMask(std::initializer_list<uint8_t> collisions) {
        uint16_t mask = 0;
        for(auto c : collisions) {
            mask |= (1 << c);
        }
        return mask;
    }

    static void drawNotchedRect(int x, int y, uint8_t w, uint8_t h, uint8_t color) {
        if (w < 3 || h < 3) {
            RenderSystem::drawRect(x, y, w, h, color);
        } else {
            RenderSystem::drawLine(x + 1, y, x + w - 2, y, color); // x2 has a -2 to account for incorrect PokittoLib implementation
            RenderSystem::drawRect(x, y + 1, w, h - 2, color);
            RenderSystem::drawLine(x + 1, y + h - 1, x + w - 2, y + h - 1, color);
        }
    }

    static void printHorizontallyCentered(int x, int y, const char * text, uint8_t color) {
        x = x - float(strlen(text) * 3.71f) / 2.0f;
        RenderSystem::print(x, y, text, color);
    }
};

#endif // HELPERS_H
