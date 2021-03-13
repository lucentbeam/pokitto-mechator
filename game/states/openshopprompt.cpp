#include "openshopprompt.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core/rendering/rendersystem.h"

#include "game/states/shop.h"

#include "game/utilities/tween.h"
#include "game/constants.h"

#include "game/ui/ui.h"

#include "game/variables.h"

static bool can_open = false;

static UIElement title_box = UIElement::getExpander(51, 35, 62, 9, Tween::Easing::OutQuad);
static UIElement alert_box = UIElement::getExpander(51, 45, 88, 9, Tween::Easing::OutQuad);

void showOpenShopPrompt()
{
    FSM::instance->go(GameStates::ShowUnlockShop);

    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(100));

    UI::setVisibility(UI::Element::UIHealthbar, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, false);

    can_open = GameVariables::hackingKits() > 0;
    title_box.setVisibility(true, uint32_t(50));
    alert_box.setVisibility(true, uint32_t(150));
}

void updateOpenShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();

    if (status.b.pressed()) {
        goGame();
    } else if (status.a.pressed()) {
        if (can_open) {
        POIs::unlockCurrent();
            showShop();
        } else {
            goGame();
            title_box.setVisibility(false);
            alert_box.setVisibility(false);
        }
    }
}

void drawOpenShopState()
{
    drawGameState();
    title_box.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        Helpers::printHorizontallyCentered(x + w/2, y + 2, "ENEMY BASE", 41);
    });
    if (!can_open) {
        alert_box.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t){
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "- NEED HACKING KIT -", 33);
        });
    } else {
        alert_box.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t){
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "USE HACKING KIT?", 37);
        });
    }
}
