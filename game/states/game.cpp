#include "game/states/game.h"


#include "core/utilities/babyfsm.h"
#include "core/utilities/fpshelper.h"

#include "core/rendering/camera.h"
#include "game/player.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/projectile.h"

#include "core/rendering/screenbuffer.h"
#include "game/entities/effects.h"
#include "game/rendering/cloudmanager.h"

#include "game/ui/ui.h"

#include "game/maps/spawnpoint.h"
#include "game/entities/enemy.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"

#include "game/constants.h"
#include "game/states/pause.h"
#include "game/maps/alertregion.h"
#include "game/maps/sequencetrigger.h"

static Player player;
//static ScreenBuffer screenbuffer;

UIElement region_indicator = UIElement::getExpander(57, 8, 30, 9, Tween::OutQuad);
const char * region_name = "";

static int8_t * life_list[7] = {nullptr};
static std::function<void()> active_callback;
static int watchcount = 0;

void goGame()
{
    FSM::instance->go(GameStates::Game);

    SpawnPoint::setActiveRegion();
    Camera::update(player.position().x(), player.position().y());
    SpawnPoint::setActiveRegion();

    UI::showHealthbar();
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);
}

void checkForCallback() {
    if (watchcount == 0) return;
    for(int i = 0; i < 7; ++i) {
        if (life_list[i] != nullptr) {
            if (*(life_list[i]) <= 0) {
                life_list[i] = nullptr;
                watchcount--;
            }
        }
    }
    if (watchcount == 0) {
        active_callback();
    }
}

void updateRegionIndicator() {
    region_indicator.update(physicsTimestep);
}


void updateGameState(FSM&) {
    SpriteWrapper::update();

    // player characters
    Soldier::update(physicsTimestep);
    Jeep::update(physicsTimestep);
    Tank::update(physicsTimestep);
    Boat::update(physicsTimestep);
    Helicopter::update(physicsTimestep);

    // enemies
    ProjectileManager::update(physicsTimestep);
    EffectManager::update(physicsTimestep);
    Enemy::update(physicsTimestep);
    Barracks::update(physicsTimestep);
    Pickups::update(physicsTimestep);
    POIs::update(physicsTimestep);
    UI::update(physicsTimestep);

    Camera::update(player.position().x(), player.position().y());
    MapManager::update();

#ifndef DEBUGS
    if (SequenceTrigger::checkForTriggers()) return;
#endif

    SpawnPoint::setActiveRegion();
//    CloudManager::update(physicsTimestep);

    checkForCallback();

    ControlStatus status = Controls::getStatus();

    updateRegionIndicator();
    if ((status.x != 0 || status.y != 0) && Player::mode() != PlayerMode::BoatMode) {
        if (checkGroundRegions(region_name)) {
            region_indicator.setMaxWidth(RenderSystem::getLineLength(region_name) + 8);
            region_indicator.showForDuration(3.0f);
        }
    }

    if (status.c.pressed()) {
        goPause();
    }
}

void drawGameState() {
    // ground layer
    MapManager::draw(true);

//    MapManager::draw(&screenbuffer);
//    RenderSystem::drawBuffer(screenbuffer.getData());

    // entities
    Barracks::draw();
    POIs::draw();
    Pickups::draw();
    Enemy::draw();
    Jeep::draw();
    Tank::draw();
    Boat::draw();
    Helicopter::drawGround();
    Soldier::draw();
    ProjectileManager::draw();
    EffectManager::draw();

    // sky layer
    MapManager::draw(false);
//    CloudManager::draw();

    ProjectileManager::drawAir();
    Helicopter::drawAir();
    Soldier::drawAir();
    Enemy::drawAir();

    // ui draw
    UI::draw();

    region_indicator.draw(true, [&](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 7) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, region_name, 10);
        }
    });

    // debug
//    static FPSHelper fps;
//    fps.update();
//    fps.draw(2, 1, 2, 9);
}

void drawShadedGame(int shading)
{
    // ground layer
    MapManager::draw(true);

//    MapManager::draw(&screenbuffer);
//    RenderSystem::drawBuffer(screenbuffer.getData());

    // entities
    POIs::draw();
    Pickups::draw();
    Enemy::draw();
    Jeep::draw();
    Tank::draw();
    Boat::draw();
    Helicopter::drawGround();
    Soldier::draw();
    ProjectileManager::draw();
    EffectManager::draw();

    // sky layer
    MapManager::draw(false);
//    CloudManager::draw();

    ProjectileManager::drawAir();
    Helicopter::drawAir();
    Enemy::drawAir();
    RenderSystem::shadeAll(shading);

    // ui draw
    UI::draw();

    // debug
//    static FPSHelper fps(10);
//    fps.update();
//    fps.draw(2, 1, 2, 9);
}

void registerCallback(std::initializer_list<int8_t *> lifes, std::function<void ()> callback)
{
    int ct = 0;
    watchcount = 0;
    for(int8_t * l : lifes) {
        life_list[ct] = l;
        ++ct;
        ++watchcount;
    }
    while (ct < 7) {
        life_list[ct] = nullptr;
        ++ct;
    }
    active_callback = callback;
}
