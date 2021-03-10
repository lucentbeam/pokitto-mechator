#include "shop.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core/rendering/rendersystem.h"

void showShop()
{
    FSM::instance->go(GameStates::ShowShop);
}

void updateShopState(FSM &fsm)
{
    static Controls ctrl;
    ControlStatus status = ctrl.getStatus();

    if (status.a.pressed()) {
        fsm.go(GameStates::Game);
    }
}

void drawShopState(RenderSystem *renderSystem)
{
    drawGameState(renderSystem);
    renderSystem->drawRect(40, 30, 30, 28, 0);

    renderSystem->print(44, 34, "shop", 41);
}
