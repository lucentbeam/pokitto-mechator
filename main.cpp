#include <math.h>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"
#include "core/utilities/babyfsm.h"

#include "game/states/game.h"
#include "game/states/opendoorprompt.h"
#include "game/states/openshopprompt.h"
#include "game/states/shop.h"
#include "game/states/pause.h"

#include "game/ui/ui.h"

#include "game/constants.h"

int main ()
{
    RenderSystem::initialize();

    FSM fsm;
    fsm.add(GameStates::Game, updateGameState, drawGameState);
    fsm.add(GameStates::ShowUnlockDoor, updateOpenDoorState, drawOpenDoorState);
    fsm.add(GameStates::ShowUnlockShop, updateOpenShopState, drawOpenShopState);
    fsm.add(GameStates::ShowShop, updateShopState, drawShopState);
    fsm.add(GameStates::Pause, updatePauseState, drawPauseState);

    int32_t gameTime = 0;
    uint32_t lastGameTime = RenderSystem::getTimeMs();

    const uint8_t maxPhysicsStepsPerFrame = 5; // prevent death spiral

    goGame();

    while (RenderSystem::running())
    {
        bool draws = RenderSystem::update();

        gameTime += RenderSystem::getTimeMs() - lastGameTime;
        lastGameTime = RenderSystem::getTimeMs();
        uint8_t step_counter = 0;
        while (gameTime > 0 && step_counter < maxPhysicsStepsPerFrame) {
            Controls::update();
            fsm.update();
            gameTime -= physicsTimestepMs;
            step_counter++;
        }

        if (draws) {
            fsm.draw(); // TODO: add frame interpolation?
        }
    }

    return 0;
}
