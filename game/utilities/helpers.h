#ifndef HELPERS_H
#define HELPERS_H

#include <cstdint>
#include <initializer_list>

#include "core/rendering/rendersystem.h"

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
};

#endif // HELPERS_H
