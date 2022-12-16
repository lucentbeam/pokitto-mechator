#include "game/states/game.h"


#include "core/utilities/babyfsm.h"
#include "core/utilities/fpshelper.h"

#include "core/rendering/camera.h"
#include "game/player.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/projectile.h"

#include "core/rendering/screenbuffer.h"
#include "game/entities/effects.h"

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

#include "game/utilities/debuglog.h"

UIElement region_indicator = UIElement::getExpander(51, 8, 30, 9, Tween::OutQuad);
const char * region_name = "";

static int8_t * life_list[7] = {nullptr};
static std::function<void()> active_callback;
static int watchcount = 0;

static bool has_update_callback = false;
static std::function<bool()> update_callback;

static bool drawflashlight;

static int from_title_counter = 0;

void goGame(bool from_title)
{
    if (from_title) {
        DebugLog::log("going to game from title");
        Enemy::clearAll();
        Barracks::clear();
        Pickups::clear();
        ProjectileManager::clear();
        SpawnPoint::reset();
        MapManager::rebuildVisibleTiles();
        Camera::update(-5000, -5000);
        Camera::stopMovement();
        Camera::shake(0.0f, 0.1f);
        SpawnPoint::setActiveRegion();
        drawflashlight = true;
        POIs::reset();
        Controls::blockControls(20);
        from_title_counter = 2;
    }

    FSM::instance->go(GameStates::Game);

    Camera::update(Player::position().x(), Player::position().y());
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

    // player characters
    Soldier::update(physicsTimestep);
    Jeep::update(physicsTimestep);
    Tank::update(physicsTimestep);
    Boat::update(physicsTimestep);
    Helicopter::update(physicsTimestep);

    SpriteWrapper::update();

    // enemies
    ProjectileManager::update(physicsTimestep);
    EffectManager::update(physicsTimestep);
    Enemy::update(physicsTimestep);
    Barracks::update(physicsTimestep);
    Pickups::update(physicsTimestep);
    POIs::update(physicsTimestep);
    UI::update(physicsTimestep);

    Camera::update(Player::position().x(), Player::position().y());
    MapManager::update();

    if (SequenceTrigger::checkForTriggers()) return;
    checkWaterSpawns();

    SpawnPoint::setActiveRegion();

    checkForCallback();
    if (has_update_callback) {
        has_update_callback = !update_callback();
    }

    updateRegionIndicator();
    if ((status.x != 0 || status.y != 0)) {
        if (checkGroundRegions(region_name, Player::mode() == BoatMode)) {
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

    if (from_title_counter > 0) {
        RenderSystem::clear(0);
        from_title_counter--;
        return;
    }
    RenderSystem::setOffset(false);
    // ground layer
    MapManager::draw(true);

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

    // tree canopy layer
    MapManager::draw(false);

    // sky layer
    ProjectileManager::drawAir();
    Helicopter::drawAir();
    Soldier::drawAir();
    Enemy::drawAir();

    if (!RegionTransitionHandler::atBoss() && isInRegion(RegionStormyCape)) {
        if (drawflashlight) Player::drawFlashlight();
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
        RenderSystem::setOffset(true);
        ProjectileManager::draw();
    }


    // ui draw
    UI::draw();

    region_indicator.draw(true, [&](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 0) {
            Helpers::printHorizontallyCentered(x + w/2, y + h/2 - 3, region_name, h > 7 ? 10 : (9 - (h % 2)*2));
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
    if (from_title_counter > 0) {
        RenderSystem::clear(0);
        from_title_counter--;
        return;
    }
    RenderSystem::setOffset(false);
    // ground layer
    MapManager::draw(true);

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

    // tree canopy layer
    MapManager::draw(false);

    // sky layer
    ProjectileManager::drawAir();
    Helicopter::drawAir();
    Soldier::drawAir();
    Enemy::drawAir();

    RenderSystem::shadeAll(shading);

    if ((isInRegion(RegionScorchedCanyons) || isInRegion(RegionFoggySwamp) || isInRegion(RegionStormyCape)) && !RegionTransitionHandler::atBoss()) RenderSystem::setOffset(true);

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
            if ((rand() % 10) < 3) {
                if (Player::mode() == BoatMode && (rand() % 100) < 15) {
                    Enemy::spawnHelicopter({locs[i].x(), locs[i].y()});
                } else {
                    Enemy::spawnBoat({locs[i].x(), locs[i].y()});
                }
            }
            else Enemy::spawnWaterMine({locs[i].x(), locs[i].y()}, 0, 0);
        }
    }
}

void setDrawFlashlight(bool val)
{
    drawflashlight = val;
}
