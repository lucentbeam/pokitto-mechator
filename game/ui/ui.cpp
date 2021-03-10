#include "ui.h"

#include "game/tilesets.h"

void UI::drawHealthBar(uint8_t current, uint8_t max, PlayerMode mode)
{
    RenderSystem::drawRect(0, 0, 7, 88, 0);
    for(uint8_t i = 0; i < max; i++) {
        int idx = i < current ? uint8_t(mode) : 0;
        RenderSystem::sprite(2, 84 - i * 3, health_pips[idx]);
    }
}
