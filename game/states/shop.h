#ifndef SHOP_H
#define SHOP_H

#include "game/enums.h"
#include "core/utilities/babyfsm.h"

void showShop();

void updateShopState(FSM &fsm);

void drawShopState(RenderSystem * renderSystem);

#endif // SHOP_H
