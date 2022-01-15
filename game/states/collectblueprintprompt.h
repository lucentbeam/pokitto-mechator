#ifndef COLLECTBLUEPRINTPROMPT_H
#define COLLECTBLUEPRINTPROMPT_H

#include "core/utilities/babyfsm.h"

#include "game/enums.h"

#include <string>

void showBlueprint(Blueprints bp);

void updateShowBlueprint(FSM&);

void drawShowBlueprint();

#endif // COLLECTBLUEPRINTPROMPT_H
