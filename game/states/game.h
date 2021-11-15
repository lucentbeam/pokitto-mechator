#ifndef GAME_H
#define GAME_H

#include "core/utilities/babyfsm.h"
#include "core//rendersystem.h"

void goGame();

void updateGameState(FSM &fsm);

void drawGameState();

void drawShadedGame(int shading = 1);

#endif // GAME_H
