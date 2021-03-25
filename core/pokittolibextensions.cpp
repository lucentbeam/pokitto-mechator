#include "pokittolibextensions.h"

#include <cstring>
#include <algorithm>

#ifndef POKITTO_SFML

#include "PokittoDisplay.h"

namespace Pokitto {

    void DisplayExtensions::shift(int16_t x, int16_t y) {
        uint8_t* m_scrbuf = Display::getBuffer();
      for (int j = 0; j < 176; j++) {
          uint8_t * loc = m_scrbuf + 110 * j;
          std::memcpy(loc, loc+x, 110-x);
      }
    }

    void DisplayExtensions::drawTile(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* tile) {
        if (y<-h || y>Display::height) return; //invisible
        if (x<-w || x>Display::width) return;  //invisible
        int screenh = std::min((int)h, Display::height- y);
        int screenw = std::min((int)w, Display::width - x);
        int screenx = x;
        int offsetx = 0;
        if (x < 0) {
            screenx = 0;
            offsetx = -x;
            screenw -= offsetx;
        }
        int screeny = y;
        int offsety = 0;
        if (y < 0) {
            screeny = 0;
            offsety = -y;
        }
        uint8_t* loc = Display::getBuffer() + 110 * screeny + screenx;
        uint8_t* tloc = const_cast<uint8_t*>(tile) + offsetx;
        for (int j = 0; j < screenh; j++) {
          std::memcpy(loc, tloc, screenw);
          loc += 110;
          tloc += w;
        }
    }

    void DisplayExtensions::drawClippedBitmap(int x, int y, int w, int h, int clipw, int cliph, const uint8_t* bitmap) {
        int16_t scrx,scry;//,scrxjump;
        int16_t xjump=0;
        int height = Display::height;
        int width = Display::width;
        uint8_t* m_scrbuf = Display::getBuffer();
        /** y clipping */
        if (y<0) {
            h += y;
            bitmap -= y*w;
            y = 0;
        } else if (y+h>height) {
            h -= (y-height);
        }
    
        /** x clipping */
        if (x<0) {
            w+=x;
            xjump = (-x);
            bitmap += xjump;
            x=0;
        } else if (x+w>width) {
            xjump=(x+w-width);
            w = width-x;
        }
    
        uint8_t* scrptr = m_scrbuf + (y*width + x);
        for (scry = y; scry < y+h; scry+=1) {
            if (scry>=height) return;
            for (scrx = x; scrx < w+x; scrx++) {
                if (scrx-x < clipw && scry-y < cliph) {
                    uint8_t sourcepixel = *bitmap;
                    uint8_t targetpixel = *scrptr;
                    if (sourcepixel != Display::invisiblecolor )
                        targetpixel = sourcepixel;
                    *scrptr = targetpixel;
                }
                bitmap++;
                scrptr++;
            }
            bitmap += xjump; // needed if horizontal clipping occurs
            scrptr = scrptr + (width - w);
        }
    }

    void DisplayExtensions::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int transparent_color, bool flip)
    {
        int16_t w = *bitmap;
        int16_t h = *(bitmap + 1);
        /** visibility check */
        if (y<-h || y>Display::height) return; //invisible
        if (x<-w || x>Display::width) return;  //invisible
        bitmap = bitmap + 2;
        if (transparent_color >= 0) {
            Display::invisiblecolor = transparent_color;
        }
        if (flip) {
            Display::drawBitmapDataXFlipped(x, y, w, h, bitmap);
        } else {
            Display::drawBitmapData(x, y, w, h, bitmap);
        }
    }

    void DisplayExtensions::drawShadow(int16_t x, int16_t y, const uint8_t *sprite, int transparent_color, const uint8_t *shading, bool flip)
    {
        const uint8_t screenwidth = 110;
        const uint8_t screenheight = 88;
        uint8_t w = sprite[0];
        uint8_t h = sprite[1];
        const uint8_t * start = sprite + 2;
        uint8_t* m_scrbuf = Display::getBuffer();
        for (int i = 0; i < w * h; ++i) {
            int idx = start[i];
            if (idx == transparent_color) {
                continue;
            }
            int dx = i % w;
            if (dx >= w) continue;
            int dy = i / w;
            int px = x + dx;
            if (flip) {
                px = x + (w - dx);
            }
            int py = y + dy;
            if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
                continue;
            }
            int col_idx = m_scrbuf[px + py * screenwidth];
            m_scrbuf[px + py * screenwidth] = shading[col_idx];
        }
    }
}

#endif
