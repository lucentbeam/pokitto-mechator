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

#include "core/palettes.h"
#include "core/audiosystem.h"

#include "game/maps/regiontransitionhandler.h"

static Player player;
//static ScreenBuffer screenbuffer;

UIElement region_indicator = UIElement::getExpander(57, 8, 30, 9, Tween::OutQuad);
const char * region_name = "";

static int8_t * life_list[7] = {nullptr};
static std::function<void()> active_callback;
static int watchcount = 0;

static bool has_update_callback = false;
static std::function<bool()> update_callback;

static bool drawflashlight;

void goGame(bool from_title)
{
    if (from_title) {
        Enemy::clearAll();
        Barracks::clear();
        Pickups::clear();
        ProjectileManager::clear();
        SpawnPoint::reset();
        MapManager::rebuildVisibleTiles();
        Camera::update(-5000, -5000);
        SpawnPoint::setActiveRegion();
        drawflashlight = true;
    }

    Controls::blockControls(2);

    FSM::instance->go(GameStates::Game);

    Camera::update(player.position().x(), player.position().y());
    SpawnPoint::setActiveRegion();
    if (from_title) {
        RegionTransitionHandler::clear();
        has_update_callback = false;
        UI::clearBoss();
    }

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

void updateGameState(FSM& fsm) {
    if (Player::dead()) {
        fsm.go(GameOverState);
        return;
    }

    ControlStatus status = Controls::getStatus();

#ifdef DEBUGS
    static int counter = 60;
    counter--;
    if (counter == 0) {
//        GameVariables::updateTime(500000);
//        GameVariables::saveGame();
//        fsm.go(GameStates::GameWonState);
    }
    if (status.f1.pressed()) {
        DebugOptions::noclip = !DebugOptions::noclip;
    }
    if (status.f2.pressed()) {
        Barracks::queryActive();
    }
#endif
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

    if (SequenceTrigger::checkForTriggers()) return;
    checkWaterSpawns();

    SpawnPoint::setActiveRegion();
//    CloudManager::update(physicsTimestep);

    checkForCallback();
    if (has_update_callback) {
        has_update_callback = !update_callback();
    }

    updateRegionIndicator();
    if ((status.x != 0 || status.y != 0) && Player::mode() != PlayerMode::BoatMode) {
        if (checkGroundRegions(region_name)) {
            if (strlen(region_name) > 0) {
                region_indicator.setMaxWidth(RenderSystem::getLineLength(region_name) + 8);
                region_indicator.showForDuration(3.0f);
            } else {
                region_indicator.setVisibility(false, uint32_t(0));
            }
        }
    }

    if (status.c.pressed()) goPause();

    RegionTransitionHandler::goRegion(currentRegion());
    RegionTransitionHandler::update();

    if (GameVariables::getGameWon()) {
        if (SequenceTrigger::checkFinalSequence()) {
            fsm.go(GameStates::GameWonState);
        }
    }
}

void drawGameState() {
    RenderSystem::setOffset(false);
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

    if (isInRegion(RegionStormyCape)) {
        if (drawflashlight && !RegionTransitionHandler::atBoss()) Player::drawFlashlight();
        const uint8_t rain[] = { 3, 3, 0, 0, 48, 0, 48, 0, 48, 0, 0};
        int x = 0;
        int y = 0;
        while (y < 85) {
            x += (rand() % 50);
            if (x > 110) {
                x -= 110;
                y += 2 + (rand() % 5);
            }
            RenderSystem::sprite(x, y, rain, 0);
        }
        if (!RegionTransitionHandler::atBoss()) RenderSystem::setOffset(true);
        if (!RegionTransitionHandler::atBoss()) ProjectileManager::draw();
    }

    // ui draw
    UI::draw();

    region_indicator.draw(true, [&](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 7) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, region_name, 10);
        }
    });

    // debug
//    Camera::debugShowRegion(true);
//    static FPSHelper fps;
//    fps.update();
//    fps.draw(2, 1, 2, 9);
}

void drawShadedGame(int shading)
{
    RenderSystem::setOffset(false);
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

    if (isInRegion(RegionStormyCape) && !RegionTransitionHandler::atBoss()) RenderSystem::setOffset(true);

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

void registerUpdateCallback(std::function<bool()> callback) {
    has_update_callback = true;
    update_callback = callback;
}

void checkWaterSpawns()
{
    if (!Camera::hasMovedRegions()) return;

    int dx = Camera::regionDeltaX();
    int dy = Camera::regionDeltaY();
    Vec2f locs[3];
    if (dx != 0 && dy != 0) {
        dx = dx > 0 ? 1 : -1;
        dy = dy > 0 ? 1 : -1;

        locs[0].set(dx, dy);
        locs[1].set(dx, 0);
        locs[2].set(0, dy);
    } else if (dx != 0) {
        dx = dx > 0 ? 1 : -1;

        locs[0].set(dx, -1);
        locs[1].set(dx, 0);
        locs[2].set(dx, 1);
    } else {
        dy = dy > 0 ? 1 : -1;

        locs[0].set(-1, dy);
        locs[1].set(0, dy);
        locs[2].set(1, dy);
    }
    for(int i = 0; i < 3; ++i) {
        locs[i] *= Camera::regionWidth();
        locs[i] += Camera::center();
        locs[i] += Vec2f((rand() % 36) - 18, (rand() % 36) - 18);
        if (MapManager::getTileAt(locs[i].x(), locs[i].y()) == 19) {
            if ((rand() % 10) < 3) Enemy::spawnBoat({locs[i].x(), locs[i].y()});
            else Enemy::spawnWaterMine({locs[i].x(), locs[i].y()}, 1, 1);
        }
    }
}

void setDrawFlashlight(bool val)
{
    drawflashlight = val;
}
