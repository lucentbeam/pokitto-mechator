#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core/controls/controls.h"

void goPause()
{
    UI::setVisibility(UI::Element::UIKeyACount, true);
    UI::setVisibility(UI::Element::UIKeyBCount, true);
    UI::setVisibility(UI::Element::UIKeyCCount, true);
    UI::setVisibility(UI::Element::UIDollarCount, true);
    UI::setVisibility(UI::Element::UIHackingKitCount, true);
    FSM::instance->go(GameStates::Pause);
}

void updatePauseState(FSM &fsm)
{

    ControlStatus status = Controls::getStatus();
    if (status.c.pressed()) {
        UI::setVisibility(UI::Element::UIKeyACount, false);
        UI::setVisibility(UI::Element::UIKeyBCount, false);
        UI::setVisibility(UI::Element::UIKeyCCount, false);
        UI::setVisibility(UI::Element::UIDollarCount, false);
        UI::setVisibility(UI::Element::UIHackingKitCount, false);
        FSM::instance->go(GameStates::Game);
    }
}

void drawPauseState()
{
    drawGameState();
}
