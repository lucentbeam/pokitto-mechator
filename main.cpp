#include <math.h>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"
#include "core/utilities/babyfsm.h"

#include "game/states/game.h"
#include "game/states/opendoorprompt.h"
#include "game/states/openshopprompt.h"
#include "game/states/shop.h"

RenderSystem renderSystem;

int main ()
{
    renderSystem.initialize();

    FSM fsm;
    fsm.add(GameStates::Game, updateGameState, drawGameState);
    fsm.add(GameStates::ShowUnlockDoor, updateOpenDoorState, drawOpenDoorState);
    fsm.add(GameStates::ShowUnlockShop, updateOpenShopState, drawOpenShopState);
    fsm.add(GameStates::ShowShop, updateShopState, drawShopState);

    int32_t gameTime = 0;
    uint32_t lastGameTime = renderSystem.getTimeMs();

    const uint8_t physicsTimestepMs = 14;
    const uint8_t maxPhysicsStepsPerFrame = 5; // prevent death spiral

    while (renderSystem.running())
    {
        bool draws = renderSystem.update();

        gameTime += renderSystem.getTimeMs() - lastGameTime;
        lastGameTime = renderSystem.getTimeMs();
        uint8_t step_counter = 0;
        while (gameTime > 0 && step_counter < maxPhysicsStepsPerFrame) {
            fsm.update();
            gameTime -= physicsTimestepMs;
            step_counter++;
        }

        if (draws) {
            fsm.draw(&renderSystem); // TODO: add frame interpolation?
        }
    }

    return 0;
}
