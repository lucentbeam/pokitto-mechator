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

Tween toLand(Tween::OutQuad, 5.0f);
const SceneWait wait0 = SceneWait(60);
const SceneFunc func0 = SceneFunc([](){ toLand.reset(); return true; });
const SceneFunc func1 = SceneFunc([](){
    int x = toLand.getInterpolationInt(-20, playerStartTileX * 6 + 3);
    int z = toLand.getInterpolationInt(60, 0);
    Soldier::setPosition({x, Soldier::position().y()});
    Soldier::setZ(z);
    return toLand.done();
});
const SceneDialogue dlog0 = SceneDialogue("Mechator, mechator!","Can you hear me?", false, false);
const SceneDialogue dlog1 = SceneDialogue("This is central.", nullptr, false, false);
const SceneDialogue dlog2 = SceneDialogue("You're in enemy","territory now.", false, false);
const SceneDialogue dlog3 = SceneDialogue("Find their shops!","Hack them. Use them.", false, false);
const SceneDialogue dlog4 = SceneDialogue("You can build your","own weapons...", false, false);
const SceneDialogue dlog5 = SceneDialogue("...and infiltrate!",nullptr, false, false);
const SceneDialogue dlog6 = SceneDialogue("Good luck.", "Central out.", false, true);

const SceneSequence scene[] = {
    {SceneSequence::DoFunction, &func0},
    {SceneSequence::DoFunction, &func1},
    {SceneSequence::Wait, &wait0 },
    {SceneSequence::ShowDialogue, &dlog0 },
    {SceneSequence::ShowDialogue, &dlog1 },
    {SceneSequence::ShowDialogue, &dlog2 },
    {SceneSequence::ShowDialogue, &dlog3 },
    {SceneSequence::ShowDialogue, &dlog4 },
    {SceneSequence::ShowDialogue, &dlog5 },
    {SceneSequence::ShowDialogue, &dlog6 },
    {SceneSequence::End, nullptr}
};

int main ()
{
    RenderSystem::initialize();
    AudioSystem::initialize();
    Soldier::health().setMax();
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
    EventScene::startScene(scene);

    while (RenderSystem::running())
    {
        if (RenderSystem::update()) {
            gameTime += RenderSystem::getTimeMs() - lastGameTime;
            lastGameTime = RenderSystem::getTimeMs();
            while (gameTime > 0) {
                Controls::update();
                fsm.update();
                gameTime -= physicsTimestepMs;
//                gameTime += RenderSystem::getTimeMs() - lastGameTime;
//                lastGameTime = RenderSystem::getTimeMs();
            }
            fsm.draw(); // TODO: add frame interpolation?
        }
    }

    return 0;
}
