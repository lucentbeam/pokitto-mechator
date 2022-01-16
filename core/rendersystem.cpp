#include "rendersystem.h"

#include "core/rendering/tiny4x7h.h"

#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>

uint8_t RenderSystem::s_clip_width;
uint8_t RenderSystem::s_clip_height;
bool RenderSystem::s_clipping = false;
bool RenderSystem::s_slim_font = false;

static bool initialized = false;

int RenderSystem::getLineLength(const char *line, int fontSize)
{
    int width = 0;
    for(char c : std::string(line)) {
        uint8_t index = uint8_t(c);
        if (index == 195) continue;
        if (index > 128) index += 64;
        index -= TinyUnicode5x8Slim[2];
        const uint8_t* bitmap = s_slim_font ? TinyUnicode5x8Slim : TinyUnicode5x8;
        uint8_t w = *bitmap;
        uint8_t h = *(bitmap + 1);
        uint8_t hbytes=0, xtra=1;
        if (h==8 || h==16) xtra=0; //don't add if exactly on byte limit
        hbytes=(h>>3)+xtra; //GLCD fonts are arranged w+1 times h/8 bytes
        //bitmap = bitmap + 3 + index * h * ((w>>3)+xtra); //add an offset to the pointer (fonts !)
        bitmap = bitmap + 4 + index * (w * hbytes + 1); //add an offset to the pointer (fonts !)
        int8_t numBytes = *bitmap++; //first byte of char is the width in bytes

        int adjustCharStep = index == 0 ? 0 : 1;
        width += (numBytes+adjustCharStep)*fontSize;
    }
    return width;
}

#ifndef DESKTOP_BUILD

#include "Pokitto.h"
#include "core/pokittolibextensions.h"

Pokitto::Core game;

uint32_t RenderSystem::getTimeMs()
{
    return initialized ? game.getTime() : 0;
}

void RenderSystem::initialize()
{
    game.begin();
    game.display.textWrap = false;
    game.display.persistence = true;
    game.display.load565Palette(palette);
    game.display.setInvisibleColor(0);
    game.display.setColor(3, backgroundColor);
    game.display.font = s_slim_font ? TinyUnicode5x8Slim : TinyUnicode5x8;
    initialized = true;
}

bool RenderSystem::running()
{
    return game.isRunning();
}

bool RenderSystem::update()
{
    return game.update();
}

//void RenderSystem::print(int x, int y, const char * line, uint8_t color, uint8_t size, uint8_t bgcol)
//{
//    game.display.fontSize = size;
//    game.display.invisiblecolor = bgcol;
//    game.display.setColor(color, bgcol);
//    game.display.setCursor(x, y);
//    game.display.write(line);
//}

void RenderSystem::pixel(int x, int y, uint8_t pixel_idx)
{
    game.display.drawPixel(x, y, pixel_idx);
}


void RenderSystem::drawCenterScaled(int xc, int yc, const uint8_t *sprite, float scale, int transparent_color, float yscale)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    Pokitto::DisplayExtensions::drawCenterScaled(xc, yc, sprite, scale, transparent_color, yscale);
}

void RenderSystem::sprite(int x, int y, const uint8_t * sprite, int transparent_color, bool flip) {
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) {
        return;
    }
    if (flip || transparent_color >= 0) {
        Pokitto::DisplayExtensions::drawBitmap(x, y, sprite, transparent_color, flip);
    } else if (s_clipping) {
        Pokitto::DisplayExtensions::drawClippedBitmap(x, y, sprite[0], sprite[1], s_clip_width, s_clip_height, sprite + 2);
    } else {
        Pokitto::DisplayExtensions::drawTile(x, y, sprite[0], sprite[1], sprite + 2);
    }
}

void RenderSystem::sprite(int x, int y, const uint8_t *sprite, int transparent_color, int target_color, int replacement_color)
{
    Pokitto::DisplayExtensions::drawBitmap(x, y, sprite, transparent_color, target_color, replacement_color);
}

void RenderSystem::sprite(int x, int y, const uint8_t *sprite, int transparent_color, int replacement_color, bool flip)
{
    Pokitto::DisplayExtensions::drawBitmap(x, y, sprite, transparent_color, replacement_color, flip);
}

void RenderSystem::spriteWrapped(int x, int y, const uint8_t *sprite, int transparent_color)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    Pokitto::DisplayExtensions::spriteWrapped(x, y, sprite, transparent_color);
}

void RenderSystem::setClip(bool clip, uint8_t clip_width, uint8_t clip_height)
{
    s_clipping = clip;
    s_clip_width = clip_width;
    s_clip_height = clip_height;
}

void RenderSystem::drawLine(int x0, int y0, int x1, int y1, uint8_t color)
{
    game.display.setColor(color);
    game.display.drawLine(x0, y0, x1, y1);
}

void RenderSystem::drawRect(int x0, int y0, int w, int h, uint8_t color)
{
    game.display.setColor(color);
    game.display.fillRect(x0, y0, w, std::max(h-1,std::min(h,1))); // PokittoLib implementation is wrong
}


void RenderSystem::drawCircle(int x, int y, int r, int c)
{
    game.display.setColor(c);
    game.display.drawCircle(x,y,r);
}

void RenderSystem::drawShadow(int x, int y, const uint8_t *sprite, int transparent_color, bool flip)
{
    Pokitto::DisplayExtensions::drawShadow(x, y, sprite, transparent_color, shading, flip);
}

void RenderSystem::shadeAll(int steps)
{
    Pokitto::DisplayExtensions::shadeAll(steps, shading);
}

void RenderSystem::clear(uint8_t idx) {
    game.display.bgcolor = idx;
    if (game.display.persistence) {
        uint8_t * m_buffer = game.display.getBuffer();
        std::fill(m_buffer, m_buffer + 110 * 88, idx);
    }
}

void RenderSystem::drawBuffer(uint8_t *buffer)
{
    std::memcpy(game.display.getBuffer(), buffer, 110*88);
}

void RenderSystem::shiftScreen(int x, int y)
{
    uint8_t * m_buffer = game.display.getBuffer();
    const int screenwidth = 110;
    const int screenheight = 88;
    const int buffer_size = screenwidth * screenheight;
    int16_t offset = (x - y * screenwidth);
    if (offset > 0) {
        offset = offset > buffer_size ? buffer_size : offset;
        std::memmove(m_buffer + offset, m_buffer, buffer_size - offset);
    } else {
        offset = offset < -buffer_size ? -buffer_size : offset;
        std::memmove(m_buffer, m_buffer - offset, buffer_size + offset);
    }
}

uint8_t * RenderSystem::getBuffer()
{
    return game.display.getBuffer();
}
#else

const uint8_t screenwidth = 110, screenheight = 88;
const bool four_bpp = false;
const int screen_scale = 3;

#ifdef SFML_CORE
#include <SFML/Graphics.hpp>
#include <iostream>

struct SfmlSystem {
    sf::RenderWindow * window;

    sf::Image screenbuffer;

    sf::Color colors[256] = {sf::Color(0, 0, 0)};

    int clearcolor = backgroundColor;

    sf::Clock clock;

    void create() {
        window = new sf::RenderWindow(sf::VideoMode(screenwidth*screen_scale,screenheight*screen_scale), "Button City Pokitto");
        window->setView(sf::View(sf::Vector2f(screenwidth/2, screenheight/2),sf::Vector2f(screenwidth, screenheight)));
        window->setFramerateLimit(60);
        for (size_t i = 0; i < sizeof(palette)/sizeof(uint16_t); ++i) {
            uint16_t color = palette[i];
            uint8_t r = (color & 0b1111100000000000) >> 11;
            uint8_t g = (color & 0b0000011111100000) >> 5;
            uint8_t b = (color & 0b0000000000011111);
            colors[i] = sf::Color(r << 3, g << 2, b << 3);
        }
        screenbuffer.create(screenwidth, screenheight, colors[0]);
    }
} sfSys;

uint8_t getPixel(int i, int j) {
    sf::Color col = sfSys.screenbuffer.getPixel(i,j);
    int idx = 0;
    for(sf::Color c : sfSys.colors) {
        if (c == col) return idx;
        ++idx;
    }
    return 0;
}

uint32_t RenderSystem::getTimeMs()
{
    return initialized ? sfSys.clock.getElapsedTime().asMilliseconds() : 0;
}

void RenderSystem::initialize() {
    sfSys.create();
    initialized = true;
}

bool RenderSystem::running() {
    return sfSys.window->isOpen();
}

#include <list>
bool RenderSystem::update() {
    static std::list<sf::Image> s_captures;
    static size_t capture_count = 0;

    sf::Event event;
    while (sfSys.window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            sfSys.window->close();
        }
        if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
            if (capture_count == 0) {
                s_captures.clear();
                capture_count = 360;
            } else {
                capture_count = 0;
                int i = 0;
                for(auto &cap : s_captures) {
                    cap.saveToFile("screenshots/capture_"+std::to_string(i)+".png");
                    ++i;
                }
                s_captures.clear();
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(sfSys.screenbuffer);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    sfSys.window->draw(sprite);
    sfSys.screenbuffer.create(screenwidth, screenheight, sfSys.colors[sfSys.clearcolor]);
    sfSys.window->display();

    if (capture_count > 0) {
        s_captures.push_back(sfSys.window->capture());
        if (s_captures.size() > capture_count) {
            s_captures.pop_front();
        }
    }
    return true;
}

void RenderSystem::pixel(int x, int y, uint8_t color)
{
    if (x < 0 || y < 0 || x >= screenwidth || y >= screenheight) {
        return;
    }
    sfSys.screenbuffer.setPixel(x, y, sfSys.colors[color]);
}

void RenderSystem::clear(uint8_t idx) {
    sfSys.clearcolor = idx;
    sfSys.screenbuffer.create(screenwidth, screenheight, sfSys.colors[sfSys.clearcolor]);
}

void RenderSystem::shiftScreen(int x, int y)
{
    uint8_t * m_buffer = const_cast<uint8_t*>(sfSys.screenbuffer.getPixelsPtr());
    const int buffer_size = screenwidth * screenheight * 4;
    int16_t offset = (x - y * screenwidth) * 4;
    if (offset > 0) {
        offset = offset > buffer_size ? buffer_size : offset;
        std::memmove(m_buffer + offset, m_buffer, buffer_size - offset);
    } else {
        offset = offset < -buffer_size ? -buffer_size : offset;
        std::memmove(m_buffer, m_buffer - offset, buffer_size + offset);
    }
}

#else

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <iostream>

struct SDLSystem {
    SDL_Window * window;

    SDL_Renderer * renderer;

    SDL_Texture * screen;

    Uint32 pixels[110*88] = { 0 };

    Uint32 colors[256] = { 0 };

    bool running = false;

    void create() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
        window = SDL_CreateWindow("Infiltrator! Rise of the Mechator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 110 * screen_scale, 88 * screen_scale, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 110, 88);
        running = true;

        for (size_t i = 0; i < sizeof(palette)/sizeof(uint16_t); ++i) {
            uint16_t color = palette[i];
            uint8_t r = (color & 0b1111100000000000) >> 11;
            r = r << 3;
            uint8_t g = (color & 0b0000011111100000) >> 5;
            g = g << 2;
            uint8_t b = (color & 0b0000000000011111);
            b = b << 3;
            colors[i] = 0;
            colors[i] |= (0b11111111) << 24;
            colors[i] |= r << 16;
            colors[i] |= g << 8;
            colors[i] |= b;
        }
    }
} sdlSys;

uint8_t getPixel(int i, int j) {
    Uint32 col = sdlSys.pixels[i + j * 110];
    int idx = 0;
    for(Uint32 c : sdlSys.colors) {
        if (c == col) return idx;
        ++idx;
    }
    return 0;
}

uint32_t RenderSystem::getTimeMs()
{
    return initialized ? SDL_GetTicks() : 0;
}

void RenderSystem::initialize() {
    sdlSys.create();
    initialized = true;
}

bool RenderSystem::running() {
    return sdlSys.running;
}

bool RenderSystem::update() {
    SDL_UpdateTexture(sdlSys.screen, NULL, sdlSys.pixels, 110 * sizeof(Uint32));
    SDL_RenderClear(sdlSys.renderer);
    SDL_RenderCopy(sdlSys.renderer, sdlSys.screen, NULL, NULL);
    SDL_RenderPresent(sdlSys.renderer);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            case SDL_QUIT:
                sdlSys.running = false;
                SDL_DestroyWindow(sdlSys.window);
                SDL_DestroyTexture(sdlSys.screen);
                SDL_DestroyRenderer(sdlSys.renderer);
                SDL_Quit();
        }
    }
    return true;
}

void RenderSystem::pixel(int x, int y, uint8_t color)
{
    if (x < 0 || y < 0 || x >= screenwidth || y >= screenheight) {
        return;
    }
    sdlSys.pixels[x + y * 110] = sdlSys.colors[color];
}

void RenderSystem::clear(uint8_t idx) {
    Uint32 val = sdlSys.colors[idx];
    Uint32 * ptr = sdlSys.pixels;
    Uint32 * end = ptr + screenwidth * screenheight;
    while(ptr != end){
        *ptr = val;
        ++ptr;
    }
}

void RenderSystem::shiftScreen(int x, int y)
{
    uint8_t * m_buffer = reinterpret_cast<uint8_t*>(sdlSys.pixels);
    const int buffer_size = screenwidth * screenheight * 4;
    int16_t offset = (x - y * screenwidth) * 4;
    if (offset > 0) {
        offset = offset > buffer_size ? buffer_size : offset;
        std::memmove(m_buffer + offset, m_buffer, buffer_size - offset);
    } else {
        offset = offset < -buffer_size ? -buffer_size : offset;
        std::memmove(m_buffer, m_buffer - offset, buffer_size + offset);
    }
}

#endif

void RenderSystem::sprite(int x, int y, const uint8_t *sprite, int transparent_color, bool flip)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    uint8_t w = sprite[0];
    uint8_t width = w;
    if (four_bpp) {
        width = w + (w % 2);
    }
    uint8_t h = sprite[1];
    const uint8_t * start = sprite + 2;
    for (int i = 0; i < width * h; ++i) {
        int dx = i % width;
        if (dx >= w) continue;
        if (s_clipping && dx > s_clip_width) continue;
        int dy = i / width;
        if (s_clipping && dy > s_clip_height) continue;
        int px = x + dx;
        int py = y + dy;
        if (flip) {
            px = x + (w - dx);
        }
        if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
            continue;
        }
        // this is for 4 bpp
        int idx;
        if (four_bpp) {
            int val = start[i/2];
            if (i % 2  == 0) {
                idx = (val & 0xf0) >> 4;
            } else {
                idx = val & 0x0f;
            }
        } else {
            idx = start[i];
        }
        if (idx == transparent_color) {
            continue;
        }
        RenderSystem::pixel(px, py, idx);
    }
}

void RenderSystem::sprite(int x, int y, const uint8_t *sprite, int transparent_color, int target_color, int replacement_color)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    uint8_t w = sprite[0];
    uint8_t width = w;
    if (four_bpp) {
        width = w + (w % 2);
    }
    uint8_t h = sprite[1];
    const uint8_t * start = sprite + 2;
    for (int i = 0; i < width * h; ++i) {
        int dx = i % width;
        if (dx >= w) continue;
        if (s_clipping && dx > s_clip_width) continue;
        int dy = i / width;
        if (s_clipping && dy > s_clip_height) continue;
        int px = x + dx;
        int py = y + dy;
        if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
            continue;
        }
        // this is for 4 bpp
        int idx;
        if (four_bpp) {
            int val = start[i/2];
            if (i % 2  == 0) {
                idx = (val & 0xf0) >> 4;
            } else {
                idx = val & 0x0f;
            }
        } else {
            idx = start[i];
        }
        if (idx == transparent_color) {
            continue;
        }
        if (idx == target_color) idx = replacement_color;
        RenderSystem::pixel(px, py, idx);
    }
}

void RenderSystem::sprite(int x, int y, const uint8_t *sprite, int transparent_color, int replacement_color, bool flip)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    uint8_t w = sprite[0];
    uint8_t width = w;
    if (four_bpp) {
        width = w + (w % 2);
    }
    uint8_t h = sprite[1];
    const uint8_t * start = sprite + 2;
    for (int i = 0; i < width * h; ++i) {
        int dx = i % width;
        if (dx >= w) continue;
        if (s_clipping && dx > s_clip_width) continue;
        int dy = i / width;
        if (s_clipping && dy > s_clip_height) continue;
        int px = x + dx;
        int py = y + dy;
        if (flip) {
            px = x + (w - dx);
        }
        if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
            continue;
        }
        // this is for 4 bpp
        int idx = start[i];
        if (idx == transparent_color) {
            continue;
        }
        idx = replacement_color;
        RenderSystem::pixel(px, py, idx);
    }
}

void RenderSystem::spriteWrapped(int x, int y, const uint8_t *sprite, int transparent_color)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    uint8_t w = sprite[0];
    uint8_t width = w;
    if (four_bpp) {
        width = w + (w % 2);
    }
    uint8_t h = sprite[1];
    const uint8_t * start = sprite + 2;
    for (int i = 0; i < width * h; ++i) {
        int dx = i % width;
        if (dx >= w) continue;
        if (s_clipping && dx > s_clip_width) continue;
        int dy = i / width;
        if (s_clipping && dy > s_clip_height) continue;
        int px = x + dx;
        int py = y + dy;
        px %= screenwidth;
//        py %= screenheight;
        if (px < 0 || py < 0 || py >= screenheight) {
            continue;
        }
        // this is for 4 bpp
        int idx;
        if (four_bpp) {
            int val = start[i/2];
            if (i % 2  == 0) {
                idx = (val & 0xf0) >> 4;
            } else {
                idx = val & 0x0f;
            }
        } else {
            idx = start[i];
        }
        if (idx == transparent_color) {
            continue;
        }
        RenderSystem::pixel(px, py, idx);
    }
}

void RenderSystem::drawCenterScaled(int xc, int yc, const uint8_t *sprite, float xscale, int transparent_color, float yscale)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    if (yscale < 0) yscale = xscale;
    uint8_t w = sprite[0];
    uint8_t h = sprite[1];
    int x = xc - int(float(w) * xscale / 2.0);
    int y = yc - int(float(h) * yscale / 2.0);
    float xstep = 1.0 / xscale;
    float ystep = 1.0f / yscale;
    const uint8_t * start = sprite + 2;
    int xct = 0;
    int yct = 0;
    for (float ycounter = 0; ycounter < h; ycounter += ystep) {
        int yidx = y + yct;
        yct++;
        if (yidx < 0 || yidx >= screenheight) continue;
        for (float xcounter = 0; xcounter < w; xcounter += xstep) {
            int xidx = x + xct;
            if (xidx < 0 || xidx >= screenwidth) continue;
            int idx = start[int(ycounter) * w + int(xcounter)];
            xct++;
            if (idx == transparent_color) continue;
            RenderSystem::pixel(xidx, yidx, idx);
        }
        xct = 0;
    }
}

void RenderSystem::setClip(bool clip, uint8_t clip_width, uint8_t clip_height)
{
    s_clipping = clip;
    s_clip_width = clip_width;
    s_clip_height = clip_height;
}

void plotLow(float x0, float y0, float x1, float y1, uint8_t color) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float delta = std::abs(dy/dx);
    float error = 0;
    int y = y0;
    for (int x = x0; x < x1; x++) {
        if (x < 0 || y < 0 || x >= screenwidth || y >= screenheight) {
            error += delta;
            if (error >= 0.5) {
                y = y + std::copysign(1.0, dy);
                error--;
            }
            continue;
        }
        RenderSystem::pixel(x, y, color);
        error += delta;
        if (error >= 0.5) {
            y = y + std::copysign(1.0, dy);
            error--;
        }
    }
}

void plotHigh(float x0, float y0, float x1, float y1, uint8_t color) {
    float dx = x1 - x0;
    float dy = y1 - y0;

    float delta = std::abs(dx/dy);
    float error = 0;
    int x = x0;
    for (int y = y0; y < y1; y++) {
        if (x < 0 || y < 0 || x >= screenwidth || y >= screenheight) {
            error += delta;
            if (error >= 0.5) {
                x = x + copysign(1.0, dx);
                error--;
            }
            continue;
        }
        RenderSystem::pixel(x, y, color);
        error += delta;
        if (error >= 0.5) {
            x = x + copysign(1.0, dx);
            error--;
        }
    }
}

void plotLine( float x0, float y0, float x1, float y1, uint8_t color) { // bresenham's algorithm
    if ((std::abs(y1-y0) < std::abs(x1-x0))) {
        if (x0 > x1) {
            plotLow(x1,y1,x0,y0,color);
        } else {
            plotLow(x0,y0,x1,y1,color);
        }
    } else {
        if (y0 > y1) {
            plotHigh(x1,y1,x0,y0,color);
        } else {
            plotHigh(x0,y0,x1,y1,color);
        }
    }
}

void RenderSystem::drawLine(int x0, int y0, int x1, int y1, uint8_t color)
{
    plotLine(x0,y0,x1,y1,color);
}

void RenderSystem::drawRect(int x0, int y0, int w, int h, uint8_t color)
{
    w -= 1;
    for (int i = x0; i < (x0 + w); ++i) {
        for (int j = y0; j < (y0 + h); j++) {
            RenderSystem::pixel(i, j, color);
        }
    }
}

void RenderSystem::drawCircle(int x, int y, int r, int c)
{
    float theta_step = 1.0f / float(r);
    float counter = 0;
    while (counter < 3.1415926 * 2) {
        pixel(x + r * std::cos(counter), y + r * std::sin(counter), c);
        counter += theta_step;
    }
}

void RenderSystem::drawShadow(int x, int y, const uint8_t *sprite, int transparent_color, bool flip)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    uint8_t w = sprite[0];
    uint8_t h = sprite[1];
    const uint8_t * start = sprite + 2;
    for (int i = 0; i < w * h; ++i) {
        int dx = i % w;
        if (dx >= w) continue;
        if (s_clipping && dx > s_clip_width) continue;
        int dy = i / w;
        if (s_clipping && dy > s_clip_height) continue;
        int px = x + dx;
        if (flip) {
            px = x + (w - dx);
        }
        int py = y + dy;
        if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
            continue;
        }
        int idx = start[i];
        if (idx == transparent_color) {
            continue;
        }
        int col_idx = getPixel(px, py);
        RenderSystem::pixel(px, py, shading[col_idx]);
    }
}

void RenderSystem::shadeAll(int steps)
{
    if (s_clipping && (s_clip_width == 0 || s_clip_height == 0)) return;
    for (int i = 0; i < 110; ++i) {
        for (int j = 0; j < 88; ++j) {
            int col_idx = getPixel(i, j);
            for(int k = 0; k < steps - 1; ++k) {
                col_idx = shading[col_idx];
            }
            RenderSystem::pixel(i, j, shading[col_idx]);
        }
    }
}

void RenderSystem::drawBuffer(uint8_t *buffer)
{
    for(int i = 0; i < 110; ++i) {
        for (int j = 0; j < 88; j++) {
            uint8_t idx = buffer[j * 110 + i];
            RenderSystem::pixel(i, j, idx);
        }
    }
}

uint8_t r_screenbuffer[110 * 88];
uint8_t *RenderSystem::getBuffer()
{
    for(int i = 0; i < 110; ++i) {
        for (int j = 0; j < 88; j++) {
            r_screenbuffer[j * 110 + i] = getPixel(i, j);
        }
    }
    return r_screenbuffer;
}

#endif


int printChar(int x, int y, uint8_t index, uint8_t color, uint8_t fontSize) {
    if (index == 195) return x;
    if (index > 128) index += 64;
    index -= TinyUnicode5x8Slim[2];
    const uint8_t* bitmap = RenderSystem::s_slim_font ? TinyUnicode5x8Slim : TinyUnicode5x8;
    uint8_t w = *bitmap;
    uint8_t h = *(bitmap + 1);
    uint8_t hbytes=0, xtra=1;
    if (h==8 || h==16) xtra=0; //don't add if exactly on byte limit
    hbytes=(h>>3)+xtra; //GLCD fonts are arranged w+1 times h/8 bytes
    //bitmap = bitmap + 3 + index * h * ((w>>3)+xtra); //add an offset to the pointer (fonts !)
    bitmap = bitmap + 4 + index * (w * hbytes + 1); //add an offset to the pointer (fonts !)
    //int8_t i, j, byteNum, bitNum, byteWidth = (w + 7) >> 3;
    int8_t i, j, numBytes;
    numBytes = *bitmap++; //first byte of char is the width in bytes
    // GLCD fonts are arranged LSB = topmost pixel of char, so its easy to just shift through the column
    uint16_t bitcolumn; //16 bits for 2x8 bit high characters

    for (i = 0; i < numBytes; i++) {
            bitcolumn = *bitmap++;
            if (hbytes == 2) bitcolumn |= (*bitmap++)<<8; // add second byte for 16 bit high fonts
            for (j = 0; j < h; j++) {
                if (bitcolumn&0x1) {
                    if (fontSize==2) {
                        RenderSystem::pixel(x + (i<<1)  , y + (j<<1),color);
                        RenderSystem::pixel(x + (i<<1)+1, y + (j<<1),color);
                        RenderSystem::pixel(x + (i<<1)  , y + (j<<1)+1,color);
                        RenderSystem::pixel(x + (i<<1)+1, y + (j<<1)+1,color);
                    } else {
                        RenderSystem::pixel(x + i, y + j,color);
                    }

                }
                bitcolumn>>=1;
            }
    }
    int adjustCharStep = index == 0 ? 0 : 1;
    return x + (numBytes+adjustCharStep)*fontSize; // for character stepping
}

void RenderSystem::print(int x, int y, const char *line, uint8_t color, uint8_t size, uint8_t)
{
    std::string l(line);
    for (auto c : l) {
        x = printChar(x, y, c, color, size);
    }
}

void RenderSystem::setFontSlim(bool slim)
{
    s_slim_font = slim;
}
