#ifndef GAME_H
#define GAME_H

#include <functional>

#include "core/utilities/babyfsm.h"
#include "core//rendersystem.h"

void goGame(bool from_title = false);

void updateGameState(FSM &fsm);

void checkWaterSpawns();

void updateRegionIndicator();

void drawGameState();

void setDrawFlashlight(bool val);

void drawShadedGame(int shading = 1);

void registerCallback(std::initializer_list<int8_t *> lifes, std::function<void()> callback);

void registerUpdateCallback(std::function<bool()> callback);

#endif // GAME_H
