
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
#include "game/states/mapviewer.h"
#include "game/states/gamewon.h"
#include "game/states/title.h"
#include "game/states/gameover.h"
#include "game/states/optionsviewer.h"

#include "game/variables.h"

#include "game/ui/ui.h"

#include "game/constants.h"
#include "game/player.h"

#include "game/utilities/debuglog.h"

struct GameData {
    int gameTime = 0;
    int lastGameTime = 0;
    FSM * fsm;
};

#ifdef __EMSCRIPTEN__
#include <unistd.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#ifdef DEBUGS
int main (int argv, char * args[])
#else
#ifndef DESKTOP_BUILD
int main (int argv, char * args[])
#elif __EMSCRIPTEN__
int main()
#else
int WinMain()
#endif
#endif
{
    GameData data;

#ifdef __EMSCRIPTEN__
EM_ASM({
    FS.mkdir("/saves");

    FS.mount(IDBFS, {}, "/saves");

    Module.fs_is_ready = 0;

    FS.syncfs(
        true,
        function(Error)
        {
            // TODO: check Error
            Module.fs_is_ready = 1;
        }
    );
});
while(!EM_ASM_INT({ return Module.fs_is_ready; })) {
    printf("Loading IDBFS...\n");
    emscripten_sleep(16);
}
#endif

    RenderSystem::initialize();
    AudioSystem::initialize();
    GameOptions::initialize();

    FSM fsm;
    fsm.add(GameStates::TitleState, Title::update, Title::draw, Title::go);
    fsm.add(GameStates::Game, updateGameState, drawGameState);
    fsm.add(GameStates::ShowUnlockDoor, updateOpenDoorState, drawOpenDoorState);
    fsm.add(GameStates::ShowUnlockShop, updateOpenShopState, drawOpenShopState);
    fsm.add(GameStates::ShowShop, updateShopState, drawShopState);
    fsm.add(GameStates::Pause, updatePauseState, drawPauseState);
    fsm.add(GameStates::ShowRepairs, updateRepairsState, drawRepairsState);
    fsm.add(GameStates::ShowBlueprint, updateShowBlueprint, drawShowBlueprint);
    fsm.add(GameStates::ShowBlueprintShop, updateBlueprintsShopState, drawBlueprintsShopState);
    fsm.add(GameStates::EventState, EventScene::update, EventScene::draw);
    fsm.add(GameStates::MapState, MapViewer::update, MapViewer::draw, MapViewer::go);
    fsm.add(GameStates::GameWonState, updateGameWonState, drawGameWonState, goGameWonState);
    fsm.add(GameStates::GameOverState, GameOver::update, GameOver::draw, GameOver::go);
    fsm.add(GameStates::GameOptionsState, OptionsViewer::update, OptionsViewer::draw, OptionsViewer::go);

    // initialize everything else
    data.lastGameTime = RenderSystem::getTimeMs();
    data.fsm = &fsm;

    while (RenderSystem::running())
    {
        if (RenderSystem::update()) {
            data.gameTime += RenderSystem::getTimeMs() - data.lastGameTime;
            data.lastGameTime = RenderSystem::getTimeMs();
            while (data.gameTime > 0) {
                Controls::update();
                GameVariables::updateTime(physicsTimestepMs);
                data.fsm->update();
                data.gameTime -= physicsTimestepMs;
                data.gameTime += RenderSystem::getTimeMs() - data.lastGameTime;
                data.lastGameTime = RenderSystem::getTimeMs();
            }
            data.fsm->draw(); // TODO: add frame interpolation?
        }
#ifdef __EMSCRIPTEN__
        emscripten_sleep(16 - data.gameTime);
#endif
    }
    return 0;
}
