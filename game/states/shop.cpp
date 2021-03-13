#include "shop.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core/rendering/rendersystem.h"

void showShop()
{
    FSM::instance->go(GameStates::ShowShop);
    UI::setVisibility(UI::Element::UIDollarCount, true, uint32_t(50));

    UI::setVisibility(UI::Element::UIHealthbar, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);
}

void updateShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();

    if (status.a.pressed()) {
        goGame();
    }
}

void drawShopState()
{
    drawGameState();
    RenderSystem::drawRect(40, 30, 30, 28, 0);

    RenderSystem::print(44, 34, "shop", 41);
}
