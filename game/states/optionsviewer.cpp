#include "optionsviewer.h"

#include "game/ui/ui.h"
#include "game/states/title.h"
#include "game/states/game.h"
#include "game/states/pause.h"
#include "game/variables.h"

static UIOptions active_option(4);

void OptionsViewer::go()
{
    active_option.setSelection(0);
    UI::setShowing(false);
}

void OptionsViewer::update(FSM &fsm)
{
    ControlStatus ctrl = Controls::getStatus();
    Title::updateTimer(active_option.update(ctrl.down.pressed(), ctrl.up.pressed()) != 0);
    if (Title::updateOptions(active_option, ctrl)) {
        UI::setShowing(true);
        GameOptions::save();
        goPause(true);
    }
}

void OptionsViewer::draw()
{
    drawShadedGame();
    RenderSystem::drawRect(0, 14, 110, 56, 0);
    Title::drawOptionScreen(active_option, -9);
}
