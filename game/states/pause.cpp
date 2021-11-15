#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core//controls.h"

static UIElement pause_prompt = UIElement::getExpander(53,41,60,11, Tween::Easing::OutQuad);

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

void updatePauseState(FSM&)
{
    ControlStatus status = Controls::getStatus();
    if (status.c.releasedWithin(20)) {
        pause_prompt.setVisibility(false);
        goGame();
    }
}

void drawPauseState()
{
    drawGameState();
    pause_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            static int counter = 0;
            counter++;
            RenderSystem::print(x + w/2 - 22, y + 3, "GAME PAUSED", counter % 60 < 48 ? 10 : 6);
        }
    });
}
