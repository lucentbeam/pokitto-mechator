#ifndef UI_H
#define UI_H

#include <cstdint>

#include "core/rendering/rendersystem.h"

enum PlayerMode : uint8_t {
    Soldier = 1,
    Jeep = 2,
    Tank = 3,
    Boat = 4,
    Helicopter = 5
};

class UI
{
public:

    static void drawHealthBar(RenderSystem * renderer, uint8_t current, uint8_t max, PlayerMode mode);
};

#endif // UI_H
