#include "rendersystem.h"

#include <cstdio>
#include <cmath>
#include <cstring>

uint8_t RenderSystem::s_clip_width;
uint8_t RenderSystem::s_clip_height;
bool RenderSystem::s_clipping = false;

#ifndef POKITTO_SFML

#include "Pokitto.h"
#include "core/pokittolibextensions.h"
#include "PokittoFonts.h"

Pokitto::Core game;

uint32_t RenderSystem::getTimeMs()
{
    return game.getTime();
}

void RenderSystem::initialize()
{
    game.begin();
    game.display.textWrap = false;
    game.display.persistence = true;
    game.display.load565Palette(palette);
    game.display.setInvisibleColor(255);
    game.display.setColor(3, backgroundColor);
    game.display.font = font3x5;
}

bool RenderSystem::running()
{
    return game.isRunning();
}

bool RenderSystem::update()
{
    return game.update();
}

void RenderSystem::print(int x, int y, const char * line, uint8_t color, uint8_t size, uint8_t bgcol)
{
    game.display.fontSize = size;
    game.display.invisiblecolor = bgcol;
    game.display.setColor(color, bgcol);
    game.display.setCursor(x, y);
    game.display.write(line);
}

void RenderSystem::pixel(int x, int y, uint8_t pixel_idx)
{
    game.display.drawPixel(x, y, pixel_idx);
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
    game.display.fillRect(x0, y0, w, h);
}


void RenderSystem::drawShadow(int x, int y, const uint8_t *sprite, int transparent_color)
{
    Pokitto::DisplayExtensions::drawShadow(x, y, sprite, transparent_color, shading);
}

void RenderSystem::clear(uint8_t idx) {
    game.display.bgcolor = idx;
}

void RenderSystem::drawBuffer(uint8_t *buffer)
{
    std::memcpy(game.display.getBuffer(), buffer, 110*88);
}
#else

#include <SFML/Graphics.hpp>

#include <iostream>

const uint8_t screenwidth = 110, screenheight = 88;
const bool four_bpp = false;
const int screen_scale = 6;

struct SfmlSystem {
    sf::RenderWindow * window;

    sf::Image screenbuffer;

    sf::Color colors[256] = {sf::Color(0, 0, 0)};

    int clearcolor = backgroundColor;

    sf::Font font;

    std::vector<sf::Text> texts;

    sf::Clock clock;

    void create() {
        window = new sf::RenderWindow(sf::VideoMode(screenwidth*screen_scale,screenheight*screen_scale), "Pokitto Testing");
        window->setView(sf::View(sf::Vector2f(screenwidth/2, screenheight/2),sf::Vector2f(screenwidth, screenheight)));
        window->setFramerateLimit(60);
        for (int i = 0; i < sizeof(palette)/sizeof(uint16_t); ++i) {
            uint16_t color = palette[i];
            uint8_t r = (color & 0b1111100000000000) >> 11;
            uint8_t g = (color & 0b0000011111100000) >> 5;
            uint8_t b = (color & 0b0000000000011111);
            colors[i] = sf::Color(r << 3, g << 2, b << 3);
            font.loadFromFile("../content/font/3x5.ttf");
        }
        screenbuffer.create(screenwidth, screenheight, colors[0]);
    }
} sfSys;

uint32_t RenderSystem::getTimeMs()
{
    return sfSys.clock.getElapsedTime().asMilliseconds();
}

void RenderSystem::initialize() {
    sfSys.create();
}

bool RenderSystem::running() {
    return sfSys.window->isOpen();
}

#include <list>
bool RenderSystem::update() {
    static std::list<sf::Image> s_captures;
    static int capture_count = 0;

    sf::Event event;
    while (sfSys.window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            sfSys.window->close();
        }
        if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
            if (capture_count == 0) {
                s_captures.clear();
                capture_count = 240;
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
    for (auto &txt : sfSys.texts) {
        sfSys.window->draw(txt);
    }
    sfSys.texts.clear();
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

void RenderSystem::print(int x, int y, const char *line, uint8_t color, uint8_t size, uint8_t bgcol)
{
    sf::Text text;
    text.setString(line);
    text.setPosition(x,y-1);
    text.setFillColor(sfSys.colors[color]);
    text.setCharacterSize(size * 5 * 20); // default renderheight is 5
    text.setScale(0.0728f, 0.06f);
    text.setFont(sfSys.font);
    sfSys.texts.push_back(text);
}

void RenderSystem::pixel(int x, int y, uint8_t color)
{
    if (x < 0 || y < 0 || x >= screenwidth || y >= screenheight) {
        return;
    }
    sfSys.screenbuffer.setPixel(x, y, sfSys.colors[color]);
}

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
        sfSys.screenbuffer.setPixel(px, py, sfSys.colors[idx]);
    }
}

void RenderSystem::setClip(bool clip, uint8_t clip_width, uint8_t clip_height)
{
    s_clipping = clip;
    s_clip_width = clip_width;
    s_clip_height = clip_height;
}


void plotLow(float x0, float y0, float x1, float y1, const sf::Color &color) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float delta = std::abs(dy/dx);
    float error = 0;
    int y = y0;
    for (int x = x0; x < x1; x++) {
        sfSys.screenbuffer.setPixel(x, y, color);
        error += delta;
        if (error >= 0.5) {
            y = y + std::copysign(1.0, dy);
            error--;
        }
    }
}

void plotHigh(float x0, float y0, float x1, float y1, const sf::Color &color) {
    float dx = x1 - x0;
    float dy = y1 - y0;

    float delta = std::abs(dx/dy);
    float error = 0;
    int x = x0;
    for (int y = y0; y < y1; y++) {
        sfSys.screenbuffer.setPixel(x, y, color);
        error += delta;
        if (error >= 0.5) {
            x = x + copysign(1.0, dx);
            error--;
        }
    }
}

void plotLine( float x0, float y0, float x1, float y1, const sf::Color &color) { // bresenham's algorithm
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
    plotLine(x0,y0,x1,y1,sfSys.colors[color]);
}

void RenderSystem::drawRect(int x0, int y0, int w, int h, uint8_t color)
{
    sf::Color c = sfSys.colors[color];
    for (int i = x0; i < (x0 + w); ++i) {
        for (int j = y0; j < (y0 + h); j++) {
            if (i >= 0 && j >= 0 && i < screenwidth && j < screenheight) {
                sfSys.screenbuffer.setPixel(i,j,c);
            }
        }
    }
}

void RenderSystem::drawShadow(int x, int y, const uint8_t *sprite, int transparent_color)
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
        int py = y + dy;
        if (px < 0 || py < 0 || px >= screenwidth || py >= screenheight) {
            continue;
        }
        int idx = start[i];
        if (idx == transparent_color) {
            continue;
        }
        sf::Color c = sfSys.screenbuffer.getPixel(px, py);
        int col_idx = 0;
        for (int i = 0; i < 255; ++i) {
            if (sfSys.colors[i] == c) {
                col_idx = i;
                break;
            }
        }
        sfSys.screenbuffer.setPixel(px, py, sfSys.colors[shading[col_idx]]);
    }
}

void RenderSystem::drawBuffer(uint8_t *buffer)
{
    for(int i = 0; i < 110; ++i) {
        for (int j = 0; j < 88; j++) {
            uint8_t idx = buffer[j * 110 + i];
            sf::Color c = sfSys.colors[idx];
            sfSys.screenbuffer.setPixel(i,j,c);
        }
    }
}

void RenderSystem::clear(uint8_t idx) {
    sfSys.clearcolor = idx;
}

#endif
