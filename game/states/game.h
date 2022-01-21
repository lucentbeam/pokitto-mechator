#ifndef GAME_H
#define GAME_H

#include <functional>

#include "core/utilities/babyfsm.h"
#include "core//rendersystem.h"

void goGame();

void updateGameState(FSM &fsm);

void updateRegionIndicator();

void drawGameState();

void drawShadedGame(int shading = 1);

void registerCallback(std::initializer_list<int8_t *> lifes, std::function<void()> callback);

#endif // GAME_H
