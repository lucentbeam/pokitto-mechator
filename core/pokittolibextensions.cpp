#include "pokittolibextensions.h"

#include <cstring>
#include <algorithm>

#ifndef DESKTOP_BUILD

#include "PokittoDisplay.h"

namespace Pokitto {

    void DisplayExtensions::shift(int16_t x, int16_t y) {
        uint8_t* m_scrbuf = Display::getBuffer();
      for (int j = 0; j < 176; j++) {
          uint8_t * loc = m_scrbuf + 110 * j;
          std::memcpy(loc, loc+x, 110-x);
      }
    }

    void DisplayExtensions::spriteWrapped(int x, int y, const uint8_t * sprite, int transparent_color) {
        // todo: optimize this
        uint8_t w = sprite[0];
        uint8_t h = sprite[1];
        const uint8_t * start = sprite + 2;
        int xct = 0;
        int yct = 0;
        uint8_t* m_scrbuf = Display::getBuffer();
        for (float ycounter = 0; ycounter < h; ycounter += 1) {
            int yidx = y + yct;
            yidx %= Display::height;
            yct++;
            xct = 0;
            if (yidx < 0) continue;
            for (float xcounter = 0; xcounter < w; xcounter += 1) {
                int xidx = x + xct;
                xidx %= Display::width;
                xct++;
                if (xidx < 0 || xidx >= Display::width) continue;
                int idx = start[int(ycounter) * w + int(xcounter)];
                if (idx == transparent_color) continue;
                uint8_t * loc = m_scrbuf + xidx + yidx * Display::width;
                *loc = idx;
            }
        }
    }

    void DisplayExtensions::drawCenterScaled(int xc, int yc, const uint8_t* sprite, float scale, int transparent_color, float yscale) {
        if (yscale < 0) yscale = scale;
        uint8_t w = sprite[0];
        uint8_t h = sprite[1];
        int x = xc - int(float(w) * scale / 2.0);
        int y = yc - int(float(h) * yscale / 2.0);
        float xstep = 1.0 / scale;
        float ystep = 1.0 / yscale;
        const uint8_t * start = sprite + 2;
        int xct = 0;
        int yct = 0;
        uint8_t* m_scrbuf = Display::getBuffer();
        for (float ycounter = 0; ycounter < h; ycounter += ystep) {
            int yidx = y + yct;
            yct++;
            xct = 0;
            if (yidx < 0 || yidx >= Display::height) continue;
            for (float xcounter = 0; xcounter < w; xcounter += xstep) {
                int xidx = x + xct;
                xct++;
                if (xidx < 0 || xidx >= Display::width) continue;
                int idx = start[int(ycounter) * w + int(xcounter)];
                if (idx == transparent_color) continue;
                uint8_t * loc = m_scrbuf + xidx + yidx * Display::width;
                *loc = idx;
            }
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
        uint8_t* start = Display::getBuffer() + 110 * screeny + screenx;
        uint8_t* end = Display::getBuffer() + 110 * (screeny + screenh) + screenx;
        uint8_t* tloc = const_cast<uint8_t*>(tile) + offsetx + offsety * w;
        for (uint8_t* loc = start; loc != end;  loc += 110) {
          std::memcpy(loc, tloc, screenw);
          tloc += w;
        }
    }

    void DisplayExtensions::fillRect(int x, int y, int w, int h, uint8_t color) {
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
        uint8_t* start = Display::getBuffer() + 110 * screeny + screenx;
        uint8_t* end = Display::getBuffer() + 110 * (screeny + screenh) + screenx;
        for (uint8_t* loc = start; loc != end;  loc += 110) {
          std::memset(loc, color, screenw);
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

    void DisplayExtensions::drawBitmap(int16_t x, int16_t y, const uint8_t* sprite, int transparent_color, int target_color, int replacement_color)
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
            int py = y + dy;
            if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
                continue;
            }
            int col_idx = m_scrbuf[px + py * screenwidth];
            if (col_idx == target_color) col_idx = replacement_color;
            m_scrbuf[px + py * screenwidth] = col_idx;
        }
    }

    void DisplayExtensions::drawBitmap(int16_t x, int16_t y, const uint8_t* sprite, int transparent_color, int replacement_color, bool flip)
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
            if (col_idx == transparent_color) continue;
            col_idx = replacement_color;
            m_scrbuf[px + py * screenwidth] = col_idx;
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

    void DisplayExtensions::incrementColors(int x, int y, const uint8_t * sprite, int transparent_color, bool flip)
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
            if (col_idx >= 65) continue;
            m_scrbuf[px + py * screenwidth] = col_idx + 65;
        }
    }

    void DisplayExtensions::shadeAll(int steps, const uint8_t *shading) {
        uint8_t* m_scrbuf = Display::getBuffer();
        uint8_t* end = m_scrbuf + 110 * 88;
        int k = 0;
        steps -= 1;
        while(m_scrbuf != end) {
            int col_idx = shading[*m_scrbuf];
            while (k < steps) {
                col_idx = shading[col_idx];
                ++k;
            }
            *m_scrbuf = col_idx;
            k = 0;
            m_scrbuf++;
        }
    }
}

#endif
