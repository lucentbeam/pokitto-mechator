#include "openshopprompt.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core/rendering/rendersystem.h"

#include "game/states/shop.h"

void showOpenShopPrompt()
{
    FSM::instance->go(GameStates::ShowUnlockShop);
}

void updateOpenShopState(FSM &fsm)
{
    static Controls ctrl;
    ControlStatus status = ctrl.getStatus();

    if (status.b.pressed()) {
        fsm.go(GameStates::Game);
    } else if (status.a.pressed()) {
        POIs::unlockCurrent();
        fsm.go(GameStates::ShowShop);
    }
}

void drawOpenShopState()
{
    drawGameState();
    RenderSystem::drawRect(40, 30, 30, 28, 0);

    RenderSystem::print(44, 34, "open shop?", 41);
}
