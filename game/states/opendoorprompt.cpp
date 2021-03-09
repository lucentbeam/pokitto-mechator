#include "opendoorprompt.h"


#include "game/states/game.h"

#include "game/enums.h"
#include "core/controls/controls.h"

#include "game/entities/pois.h"

POIType currentDoor;

void showOpenDoorPrompt(POIType door) {
    if (door == POIType::Shop) {
        return;
    }
    FSM::instance->go(GameStates::ShowUnlockDoor);
    currentDoor = door;
}

void updateOpenDoorState(FSM &fsm) {
    static Controls ctrl;
    ControlStatus status = ctrl.getStatus();

    if (status.b.pressed()) {
        fsm.go(GameStates::Game);
    } else if (status.a.pressed()) {
        POIs::unlockCurrent();
        fsm.go(GameStates::Game);
    }
}

void drawOpenDoorState(RenderSystem * renderSystem) {
    drawState(renderSystem);
    renderSystem->drawRect(40, 30, 30, 28, 0);

    char buf[3];
    sprintf(buf, "%d", int(currentDoor));
    renderSystem->print(44, 34, buf, 41);
}
