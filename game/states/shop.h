#ifndef SHOP_H
#define SHOP_H

#include "game/enums.h"
#include "core/utilities/babyfsm.h"

void showShop(bool from_repairs = false);

void updateShopState(FSM &fsm);

void drawShopState();

#endif // SHOP_H
