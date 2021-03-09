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

Player player;
ScreenBuffer screenbuffer;

void updateState(FSM &fsm) {
    player.update(0.014f);
    ProjectileManager::update(0.014f);
    EffectManager::update(0.014f);
    Enemy::updateMechs(0.014f);
    Barracks::update(0.014f);
    Pickups::update(0.014f);
    POIs::update(0.014f);

    Camera::update(player.position().x(), player.position().y());

    SpawnPoint::setActiveRegion();
//    CloudManager::update(0.014f);
}

void drawState(RenderSystem * renderSystem) {
    static FPSHelper fps(10);

    MapManager::draw(&screenbuffer);
    renderSystem->drawBuffer(screenbuffer.getData());
    Pickups::draw(renderSystem);
    POIs::draw(renderSystem);
    Enemy::drawMechs(renderSystem);
    player.draw(renderSystem);

    ProjectileManager::draw(renderSystem);
    EffectManager::draw(renderSystem);

    // sky layer
    MapManager::draw(renderSystem, false);

    PlayerMode mode = Player::mode();
    UI::drawHealthBar(renderSystem, mode == PlayerMode::Soldier ? Player::s_stats.health_soldier.value() : Player::s_stats.health_jeep.value(), mode == PlayerMode::Soldier ? Player::s_stats.health_soldier.max() : Player::s_stats.health_jeep.max(), mode);

//    CloudManager::draw(&renderSystem);
    fps.update(renderSystem);
    fps.draw(renderSystem, 8, 82, 9);
    fps.draw(renderSystem, 8, 81, 37);
}
