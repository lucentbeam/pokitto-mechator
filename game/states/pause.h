#ifndef PAUSE_H
#define PAUSE_H

#include "core/utilities/babyfsm.h"
#include "core/rendering/rendersystem.h"

void goPause();

void updatePauseState(FSM &fsm);

void drawPauseState();

#endif // PAUSE_H
