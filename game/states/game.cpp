#include "game/states/game.h"


#include "core/utilities/babyfsm.h"
#include "core/utilities/fpshelper.h"

#include "game/rendering/camera.h"
#include "game/player.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/projectile.h"

#include "core/rendering/screenbuffer.h"
#include "game/entities/effects.h"
#include "game/rendering/cloudmanager.h"

#include "game/ui/ui.h"

#include "game/maps/spawnpoint.h"
#include "game/entities/enemymech.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"

#include "game/constants.h"
#include "game/states/pause.h"

static Player player;
static ScreenBuffer screenbuffer;

void goGame()
{
    FSM::instance->go(GameStates::Game);

    UI::setVisibility(UI::Element::UIHealthbar, true);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);
}

void updateGameState(FSM&) {
    player.update(physicsTimestep);
    ProjectileManager::update(physicsTimestep);
    EffectManager::update(physicsTimestep);
    Enemy::updateMechs(physicsTimestep);
    Barracks::update(physicsTimestep);
    Pickups::update(physicsTimestep);
    POIs::update(physicsTimestep);

    Camera::update(player.position().x(), player.position().y());

    SpawnPoint::setActiveRegion();
//    CloudManager::update(physicsTimestep);

    ControlStatus status = Controls::getStatus();
    if (status.c.pressed()) {
        goPause();
    }
}

void drawGameState() {
    static FPSHelper fps(10);

    // ground layer
    MapManager::draw(&screenbuffer);
    RenderSystem::drawBuffer(screenbuffer.getData());

    // entities
    Pickups::draw();
    POIs::draw();
    Enemy::drawMechs();
    player.draw();
    ProjectileManager::draw();
    EffectManager::draw();

    // sky layer
    MapManager::draw(false);
    //    CloudManager::draw();

    // ui draw
    UI::draw();

    // debug
    fps.update();
    fps.draw(8, 82, 9);
    fps.draw(8, 81, 37);
}
