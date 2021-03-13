#include "openshopprompt.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core/rendering/rendersystem.h"

#include "game/states/shop.h"

#include "game/utilities/tween.h"
#include "game/constants.h"

#include "game/ui/ui.h"

void showOpenShopPrompt()
{
    FSM::instance->go(GameStates::ShowUnlockShop);

    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(100));

    UI::setVisibility(UI::Element::UIHealthbar, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, false);
}

void updateOpenShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();

    if (status.b.pressed()) {
        goGame();
    } else if (status.a.pressed()) {
        POIs::unlockCurrent();
        showShop();
    }
}

void drawOpenShopState()
{
    drawGameState();
}
