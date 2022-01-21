#include <math.h>

#include "core//rendersystem.h"
#include "core//controls.h"
#include "core/utilities/babyfsm.h"
#include "core/audiosystem.h"

#include "game/states/game.h"
#include "game/states/opendoorprompt.h"
#include "game/states/openshopprompt.h"
#include "game/states/shop.h"
#include "game/states/pause.h"
#include "game/states/repairs.h"
#include "game/states/collectblueprintprompt.h"
#include "game/states/blueprints.h"
#include "game/states/eventscene.h"

#include "game/ui/ui.h"

#include "game/constants.h"
#include "game/player.h"

#include "game/maps/spawnpoints.h"

int main ()
{
    RenderSystem::initialize();
    AudioSystem::initialize();
    Soldier::health().setMax();


#ifdef DEBUGS
    std::cout << sizeof(Projectile) << std::endl;
    std::cout << sizeof(Effect) << std::endl;
    std::cout << sizeof(EnemyMech) << std::endl;
    std::cout << sizeof(EnemyTank) << std::endl;
    std::cout << sizeof(Steering) << std::endl;
    std::cout << sizeof(EnemyBomber) << std::endl;
    std::cout << sizeof(EnemyLasers) << std::endl;
    std::cout << sizeof(EnemyHelicopter) << std::endl;
    std::cout << sizeof(EnemyTurret) << std::endl;

//    Soldier::setPosition({73 * 6, 110 * 6}); // canyons
//    Soldier::setPosition({36 * 6, 8 * 6}); // tutorial island
    Soldier::setPosition({38 * 6, 118 * 6}); // tank factory
//    Soldier::setPosition({138 * 6, 195 * 6}); // boat yard
#endif

    Camera::update(Soldier::position().x(), Soldier::position().y());

    FSM fsm;
    fsm.add(GameStates::Game, updateGameState, drawGameState);
    fsm.add(GameStates::ShowUnlockDoor, updateOpenDoorState, drawOpenDoorState);
    fsm.add(GameStates::ShowUnlockShop, updateOpenShopState, drawOpenShopState);
    fsm.add(GameStates::ShowShop, updateShopState, drawShopState);
    fsm.add(GameStates::Pause, updatePauseState, drawPauseState);
    fsm.add(GameStates::ShowRepairs, updateRepairsState, drawRepairsState);
    fsm.add(GameStates::ShowBlueprint, updateShowBlueprint, drawShowBlueprint);
    fsm.add(GameStates::ShowBlueprintShop, updateBlueprintsShopState, drawBlueprintsShopState);
    fsm.add(GameStates::EventState, EventScene::update, EventScene::draw);

    int32_t gameTime = 0;
    uint32_t lastGameTime = RenderSystem::getTimeMs();

    goGame();

    while (RenderSystem::running())
    {
        if (RenderSystem::update()) {
            gameTime += RenderSystem::getTimeMs() - lastGameTime;
            lastGameTime = RenderSystem::getTimeMs();
//            int count = 0;
            while (gameTime > 0) {// && count < 2) {
                Controls::update();
                fsm.update();
                gameTime -= physicsTimestepMs;
//                count++;
//                gameTime += RenderSystem::getTimeMs() - lastGameTime;
//                lastGameTime = RenderSystem::getTimeMs();
            }
//            while (gameTime > physicsTimestepMs) {
//                gameTime -= physicsTimestepMs;
//            }

            fsm.draw(); // TODO: add frame interpolation?
        }
    }

    return 0;
}
