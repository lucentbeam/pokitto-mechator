#ifndef GAME_H
#define GAME_H

#include "core/utilities/babyfsm.h"
#include "core/rendering/rendersystem.h"

void updateGameState(FSM &fsm);

void drawGameState(RenderSystem * renderSystem);

#endif // GAME_H
