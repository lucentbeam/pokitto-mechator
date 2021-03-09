#ifndef GAME_H
#define GAME_H

#include "core/utilities/babyfsm.h"
#include "core/rendering/rendersystem.h"

void updateState(FSM &fsm);

void drawState(RenderSystem * renderSystem);

#endif // GAME_H
