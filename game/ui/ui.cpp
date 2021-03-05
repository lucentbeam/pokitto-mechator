#include "ui.h"

#include "game/tilesets.h"

void UI::drawHealthBar(RenderSystem *renderer, uint8_t current, uint8_t max, PlayerMode mode)
{
    renderer->drawRect(0, 0, 7, 88, 0);
    for(uint8_t i = 0; i < max; i++) {
        int idx = i < current ? uint8_t(mode) : 0;
        renderer->sprite(2, 84 - i * 3, health_pips[idx]);
    }
}
