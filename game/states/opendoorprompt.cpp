#include "opendoorprompt.h"


#include "game/states/game.h"

#include "game/enums.h"
#include "core/controls/controls.h"

#include "game/entities/pois.h"
#include "game/ui/ui.h"

static POIType currentDoor;

void showOpenDoorPrompt(POIType door) {
    if (door == POIType::Shop) {
        return;
    }
    FSM::instance->go(GameStates::ShowUnlockDoor);
    currentDoor = door;

    UI::setVisibility(UI::Element::UIHealthbar, false);
    UI::setVisibility(UI::Element::UIKeyACount, door == DoorA);
    UI::setVisibility(UI::Element::UIKeyBCount, door == DoorB);
    UI::setVisibility(UI::Element::UIKeyCCount, door == DoorC);
    UI::setVisibility(UI::Element::UIDollarCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);
}

void updateOpenDoorState(FSM&) {
    ControlStatus status = Controls::getStatus();

    if (status.b.pressed()) {
        goGame();
    } else if (status.a.pressed()) {
        POIs::unlockCurrent();
        goGame();
    }
}

void drawOpenDoorState() {
    drawGameState();
    RenderSystem::drawRect(40, 30, 30, 28, 0);

    char buf[3];
    sprintf(buf, "%d", int(currentDoor));
    RenderSystem::print(44, 34, buf, 41);
}
