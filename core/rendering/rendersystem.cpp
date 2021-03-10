#include "rendersystem.h"

const uint16_t palette[] = {
0,59847,7662,19676,65110,10501,64323,38605,36511,14760,62626,52973,16711,25260,65029,12711,27118,37741,31112,12905,37621,44207,39463,21484,44094,27244,52039,38224,60797,31633,58505,46546,29162,40278,64970,2796,41549,50938,18916,3153,52015,65535,25382,3443,60435,26916,42312,14103,33003,45510,55049,36828,49450,60006,65520,12682,62063,62441,4809,19022,62480,43270,9356,19254,64818,
};
const uint8_t backgroundColor = 26;

#include <cstdio>
#include <cmath>

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
    if (m_clipping && (m_clip_width == 0 || m_clip_height == 0)) {
        return;
    }
    if (flip || transparent_color >= 0) {
        Pokitto::DisplayExtensions::drawBitmap(x, y, sprite, transparent_color, flip);
    } else if (m_clipping) {
        Pokitto::DisplayExtensions::drawClippedBitmap(x, y, sprite[0], sprite[1], m_clip_width, m_clip_height, sprite + 2);
    } else {
        Pokitto::DisplayExtensions::drawTile(x, y, sprite[0], sprite[1], sprite + 2);
    }
}

void RenderSystem::setClip(bool clip, uint8_t clip_width, uint8_t clip_height)
{
    m_clipping = clip;
    m_clip_width = clip_width;
    m_clip_height = clip_height;
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

void RenderSystem::clear(uint8_t idx) {
    game.display.bgcolor = idx;
}

#else

#include <SFML/Graphics.hpp>

#include <iostream>

const uint8_t screenwidth = 110, screenheight = 88;
const bool four_bpp = false;

struct SfmlSystem {
    sf::RenderWindow * window;

    sf::Image screenbuffer;

    sf::Color colors[256] = {sf::Color(0, 0, 0)};

    int clearcolor = backgroundColor;

    sf::Font font;

    std::vector<sf::Text> texts;

    sf::Clock clock;

    void create() {
        window = new sf::RenderWindow(sf::VideoMode(screenwidth*6,screenheight*6), "Pokitto Testing");
        window->setView(sf::View(sf::Vector2f(screenwidth/2, screenheight/2),sf::Vector2f(screenwidth, screenheight)));
        window->setFramerateLimit(60);
        for (int i = 0; i < sizeof(palette)/sizeof(uint16_t); i++) {
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

bool RenderSystem::update() {
    sf::Event event;
    while (sfSys.window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            sfSys.window->close();
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
    if (m_clipping && (m_clip_width == 0 || m_clip_height == 0)) return;
    uint8_t w = sprite[0];
    uint8_t width = w + (w % 2);
    uint8_t h = sprite[1];
    const uint8_t * start = sprite + 2;
    for (int i = 0; i < width * h; i++) {
        int dx = i % width;
        if (dx >= w) continue;
        if (m_clipping && dx > m_clip_width) continue;
        int dy = i / width;
        if (m_clipping && dy > m_clip_height) continue;
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
    m_clipping = clip;
    m_clip_width = clip_width;
    m_clip_height = clip_height;
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
    for (int i = x0; i < (x0 + w); i++) {
        for (int j = y0; j < (y0 + h); j++) {
            sfSys.screenbuffer.setPixel(i,j,c);
        }
    }
}

void RenderSystem::clear(uint8_t idx) {
    sfSys.clearcolor = idx;
}

#endif
