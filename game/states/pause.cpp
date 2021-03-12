#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core/controls/controls.h"

void goPause()
{
    UI::setVisibility(UI::Element::UIKeyACount, true, uint32_t(0));
    UI::setVisibility(UI::Element::UIKeyBCount, true, uint32_t(100));
    UI::setVisibility(UI::Element::UIKeyCCount, true, uint32_t(200));
    UI::setVisibility(UI::Element::UIDollarCount, true, uint32_t(250));
    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(350));
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
