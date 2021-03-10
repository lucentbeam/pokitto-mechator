#ifndef OPENSHOPPROMPT_H
#define OPENSHOPPROMPT_H

#include "game/enums.h"
#include "core/utilities/babyfsm.h"

void showOpenShopPrompt();

void updateOpenShopState(FSM &fsm);

void drawOpenShopState(RenderSystem * renderSystem);

#endif // OPENSHOPPROMPT_H
