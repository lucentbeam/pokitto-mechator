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
    ControlStatus status = Controls::getStatus();

    if (status.a.pressed()) {
        fsm.go(GameStates::Game);
    }
}

void drawShopState()
{
    drawGameState();
    RenderSystem::drawRect(40, 30, 30, 28, 0);

    RenderSystem::print(44, 34, "shop", 41);
}
