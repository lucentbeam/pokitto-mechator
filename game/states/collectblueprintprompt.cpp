#include "collectblueprintprompt.h"

#include "game/states/game.h"

#include "game/enums.h"
#include "core//controls.h"

#include "game/entities/pois.h"
#include "game/ui/ui.h"
#include "game/variables.h"
#include "core/audiosystem.h"

static UIElement header_prompt = UIElement::getExpander(55, 37, 94, 9, Tween::Easing::OutQuad);
static UIElement bp_prompt = UIElement::getExpander(55, 48, 94, 9, Tween::Easing::OutQuad);

bool showing = false;
Blueprints bp_showing;

void showBlueprint(Blueprints bp)
{
    AudioSystem::play(sfxGetItem);
    FSM::instance->go(GameStates::ShowBlueprint);

    showing = true;
    bp_showing = bp;
    header_prompt.setVisibility(true, uint32_t(0));
    bp_prompt.setVisibility(true, uint32_t(0));
}

void updateShowBlueprint(FSM &)
{
    if (header_prompt.active()) return;
    if (!showing && header_prompt.done()) {
        FSM::instance->go(GameStates::Game);
        return;
    }
    ControlStatus ctrl = Controls::getStatus();
    if (ctrl.a.pressed()) {
        AudioSystem::play(sfxConfirm);
        header_prompt.setVisibility(false, uint32_t(0));
        bp_prompt.setVisibility(false, uint32_t(0));
        showing = false;
    }
}

void drawShowBlueprint()
{
    drawShadedGame();
    header_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, "Blueprint found!", 49);
        }
    });
    bp_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, bp_names[int(bp_showing)], 10);
        }
    });
}
