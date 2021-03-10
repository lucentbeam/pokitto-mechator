#ifndef UI_H
#define UI_H

#include "core/rendering/rendersystem.h"
#include "game/enums.h"

class UI
{
public:

    static void drawHealthBar(uint8_t current, uint8_t max, PlayerMode mode);
};

#endif // UI_H
