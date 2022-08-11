#include "helpers.h"

namespace  Helpers {

    uint16_t getMask(std::initializer_list<uint8_t> collisions) {
        uint16_t mask = 0;
        for(auto c : collisions) {
            mask |= (1 << c);
        }
        return mask;
    }

    void drawNotchedRect(int x, int y, uint8_t w, uint8_t h, uint8_t color) {
#if DESKTOP_BUILD
        w += 1;
#endif
        if (w < 3 || h < 3) {
            RenderSystem::drawRect(x, y, w, h, color);
        } else {
            RenderSystem::drawLine(x + 1, y, x + w - 2, y, color); // x2 has a -2 to account for incorrect PokittoLib implementation
            RenderSystem::drawRect(x, y + 1, w, h - 2, color);
            RenderSystem::drawLine(x + 1, y + h - 1, x + w - 2, y + h - 1, color);
        }
    }

    void printHorizontallyCentered(int x, int y, const char *text, uint8_t color) {
        x = x - RenderSystem::getLineLength(text)/2;
        RenderSystem::print(x, y, text, color);
    }

    void drawRLE(int x, int y, const uint8_t *sprite, int transparent, int frame, uint8_t * buffer)
    {
        const uint8_t * input = sprite;
        while (frame > 0) {
            input += input[0];
        }
        if (frame < 0) {
            input--;
        }
        uint8_t w = input[1];
        uint8_t h = input[2];
        input += 3;
        if (buffer == nullptr) buffer = RenderSystem::getBuffer();
        int col = *input++;
        int counter = *input++;
        int yidx, xidx;
        for (int ycounter = 0; ycounter < h; ++ycounter) {
            yidx = y + ycounter;
            if (yidx >= 88) return;
            for (int xcounter = 0; xcounter < w; ++xcounter) {
                counter--;
                int c = col;
                if (counter <= 0) {
                    col = *input++;
                    counter = *input++;
                }
                if (yidx < 0) continue;
                xidx = x + xcounter;
                if (xidx < 0 || xidx >= 110) continue;
                if (c == transparent) continue;
                buffer[xidx + yidx * 110] = c;
    #ifdef DESKTOP_BUILD
                RenderSystem::pixel(xidx, yidx, c);
    #endif
            }
        }
    }

}
