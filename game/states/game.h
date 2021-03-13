#ifndef GAME_H
#define GAME_H

#include "core/utilities/babyfsm.h"
#include "core/rendering/rendersystem.h"

void goGame();

void updateGameState(FSM &fsm);

void drawGameState();

#endif // GAME_H
