#include <math.h>

#include "core//rendersystem.h"
#include "core//controls.h"
#include "core/utilities/babyfsm.h"

#include "game/states/game.h"
#include "game/states/opendoorprompt.h"
#include "game/states/openshopprompt.h"
#include "game/states/shop.h"
#include "game/states/pause.h"
#include "game/states/repairs.h"

#include "game/ui/ui.h"

#include "game/constants.h"
#include "game/player.h"

int main ()
{
    RenderSystem::initialize();
    Soldier::health().setMax();
    Camera::update(Soldier::position().x(), Soldier::position().y());

    FSM fsm;
    fsm.add(GameStates::Game, updateGameState, drawGameState);
    fsm.add(GameStates::ShowUnlockDoor, updateOpenDoorState, drawOpenDoorState);
    fsm.add(GameStates::ShowUnlockShop, updateOpenShopState, drawOpenShopState);
    fsm.add(GameStates::ShowShop, updateShopState, drawShopState);
    fsm.add(GameStates::Pause, updatePauseState, drawPauseState);
    fsm.add(GameStates::ShowRepairs, updateRepairsState, drawRepairsState);

    int32_t gameTime = 0;
    uint32_t lastGameTime = RenderSystem::getTimeMs();

    goGame();

    while (RenderSystem::running())
    {
        if (RenderSystem::update()) {
            gameTime += RenderSystem::getTimeMs() - lastGameTime;
            lastGameTime = RenderSystem::getTimeMs();
            while (gameTime > 0) {
                Controls::update();
                fsm.update();
                gameTime -= physicsTimestepMs;
                gameTime += RenderSystem::getTimeMs() - lastGameTime;
                lastGameTime = RenderSystem::getTimeMs();
            }
            fsm.draw(); // TODO: add frame interpolation?
        }
    }

    return 0;
}
