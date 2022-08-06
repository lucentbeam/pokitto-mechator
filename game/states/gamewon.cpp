#include "gamewon.h"

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

const uint16_t * palette;
uint16_t next[64];

void goGameWonState()
{
    EffectManager::createExplosionBig(Player::position());
    AudioSystem::play(sfxExplosionBig);
    palette = RenderSystem::getPalette();
}

void updateGameWonState(FSM &fsm)
{
    static float f = 1.5f;
    f -= physicsTimestep / 2.5f;
    float frac = f;
    if (f < 0) frac = 0;
    if (f > 1.0) frac = 1.0;
    SpriteWrapper::update();
    Player::updateCounter();

    EffectManager::update(physicsTimestep);
    Pickups::update(physicsTimestep);
    POIs::update(physicsTimestep);
    UI::update(physicsTimestep);

    for (int i = 0; i < 64; ++i) {
        next[i] =
                (int(float((palette[i] & 0b1111100000000000) >> 11) * frac) << 11) |
                (int(float((palette[i] & 0b0000011111100000) >> 5) * frac) << 5) |
                (int(float(palette[i] & 0b0000000000011111) * frac));
    }
    RenderSystem::setPalette(next);

    MapManager::update();
}

void drawGameWonState()
{
    MapManager::draw(true);

    Barracks::draw();
    POIs::draw();
    Pickups::draw();
    Enemy::draw();
    if (Player::mode() != JeepMode) Jeep::draw();
    if (Player::mode() != TankMode) Tank::draw();
    if (Player::mode() != BoatMode) Boat::draw();
    Helicopter::drawGround();
    if (Player::mode() != SoldierMode) Soldier::draw();
    EffectManager::draw();

    MapManager::draw(false);

    //Helicopter::drawAir();
    Soldier::drawAir();
    Enemy::drawAir();
}

