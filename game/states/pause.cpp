#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core/controls/controls.h"

static UIElement pause_prompt(23, 36, 60, 11, 53, 41, 0, 0, Tween::Easing::OutQuad);

void goPause()
{
    UI::setVisibility(UI::Element::UIKeyACount, true, uint32_t(0));
    UI::setVisibility(UI::Element::UIKeyBCount, true, uint32_t(50));
    UI::setVisibility(UI::Element::UIKeyCCount, true, uint32_t(100));
    UI::setVisibility(UI::Element::UIDollarCount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(200));
    pause_prompt.setVisibility(true, uint32_t(0));
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
        pause_prompt.setVisibility(false);
        FSM::instance->go(GameStates::Game);
    }
}

void drawPauseState()
{
    drawGameState();
    pause_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            static int counter = 0;
            counter++;
            RenderSystem::print(x + w/2 - 22, y + 3, "GAME PAUSED", counter % 60 < 48 ? 41 : 33);
        }
    });
}
