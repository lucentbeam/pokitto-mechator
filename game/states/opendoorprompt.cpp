#include "opendoorprompt.h"


#include "game/states/game.h"

#include "game/enums.h"
#include "core//controls.h"

#include "game/entities/pois.h"
#include "game/ui/ui.h"
#include "game/variables.h"
#include "core/audiosystem.h"

static POIType currentDoor;

static bool can_open = false;

static UIElement prompt = UIElement::getExpander(55, 44, 90, 9, Tween::Easing::OutQuad);
static char not_enough_text[24];
static char open_text[17];
static UIOptions yes_no(2);

void showOpenDoorPrompt(POIType door) {
    if (door == POIType::Shop) {
        return;
    }
    FSM::instance->go(GameStates::ShowUnlockDoor);
    currentDoor = door;

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIKeyACount, door == DoorA);
    UI::setVisibility(UI::Element::UIKeyBCount, door == DoorB);
    UI::setVisibility(UI::Element::UIKeyCCount, door == DoorC);
    UI::setVisibility(UI::Element::UIDollarCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);

    switch (currentDoor) {
    case DoorA:
        can_open = GameVariables::keysA();
        sprintf(not_enough_text, "- NEED KEY TOKEN [%d] -", 1);
        sprintf(open_text, "USE A TOKEN [%d]?", 1);
        break;
    case DoorB:
        can_open = GameVariables::keysB();
        sprintf(not_enough_text, "- NEED KEY TOKEN [%d] -", 2);
        sprintf(open_text, "USE A TOKEN [%d]?", 2);
        break;
    case DoorC:
        can_open = GameVariables::keysC();
        sprintf(not_enough_text, "- NEED KEY TOKEN [%d] -", 3);
        sprintf(open_text, "USE A TOKEN [%d]?", 3);
        break;
    default:
        can_open = false;
        break;
    }
    if (can_open) {
        AudioSystem::play(sfxSelect);
    } else {
        AudioSystem::play(sfxDeny);
    }

    prompt.setVisibility(true);
    yes_no.reset();
}

void updateOpenDoorState(FSM&) {
    ControlStatus status = Controls::getStatus();

    if (can_open) {
        yes_no.update(status.down.pressed(), status.up.pressed());
        if (status.a.pressed()) {
            if (yes_no.activeIndex() == 0) {
                prompt.setVisibility(false);
                goGame();
            } else {
                switch (currentDoor) {
                case DoorA:
                    GameVariables::changeKeysA(-1);
                    break;
                case DoorB:
                    GameVariables::changeKeysB(-1);
                    break;
                case DoorC:
                    GameVariables::changeKeysC(-1);
                    break;
                default:
                    break;
                }
                POIs::unlockCurrent();
                goGame();
            }
        } else if (status.b.pressed()) {
            prompt.setVisibility(false);
            goGame();
        }
    } else {
        if (status.b.pressed() || status.a.pressed()) {
            prompt.setVisibility(false);
            goGame();
        }
    }
}

void drawOpenDoorState() {
    drawShadedGame();
    prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, can_open ? open_text : not_enough_text, can_open ? 10 : 6);
            if (can_open) {
                yes_no.foreach([](uint8_t idx, bool active) {
                    Helpers::drawNotchedRect(46, 50 + idx * 8, 20, 7, 0);
                    RenderSystem::sprite(38, 50 + idx * 8, poi[active ? 1 : 0]);
                    Helpers::printHorizontallyCentered(56, 50 + idx * 8, idx == 0 ? "NO" : "YES", active ? 10 : 6);
                });
            }
        }
    });
}
