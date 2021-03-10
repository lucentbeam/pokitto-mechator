#include "openshopprompt.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core/rendering/rendersystem.h"

#include "game/states/shop.h"

#include "game/utilities/tween.h"

Tween open_tween(Tween::Easing::OutQuad, 0.15f);

void showOpenShopPrompt()
{
    FSM::instance->go(GameStates::ShowUnlockShop);
    open_tween.reset();
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

    float height = open_tween.getInterpolation(0, 22);
    float width = open_tween.getInterpolation(0, 40);
    RenderSystem::drawRect(40 + (40 - width)/2, 30 + (22-height)/2, width, height, 0);

    if (height > 15) {
        RenderSystem::print(44, 34, "open shop?", 41);
    }
}
