#ifndef OPENDOORPROMPT_H
#define OPENDOORPROMPT_H

#include "game/enums.h"
#include "core/utilities/babyfsm.h"

void showOpenDoorPrompt(POIType door);

void updateOpenDoorState(FSM &fsm);

void drawOpenDoorState();

#endif // OPENDOORPROMPT_H
