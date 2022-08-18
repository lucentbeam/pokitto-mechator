#include "gameover.h"

#include "core/palettes.h"
#include "core/audiosystem.h"

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

static float go_f = 1.4f;
const uint16_t * go_palette;
uint16_t go_next[65];

void GameOver::go()
{
    EffectManager::createExplosionBig(Player::position() - Vec2i(7, 7));
    AudioSystem::play(sfxExplosionBig);
    go_palette = RenderSystem::getPalette();
    AudioSystem::playSong(musNone);
    go_f = 1.4f;
}

void GameOver::update(FSM &fsm)
{
    float frac = go_f;
    go_f -= physicsTimestep / 1.0f;
    if (go_f < 0) frac = 0;
    if (go_f > 1.0) frac = 1.0;
    SpriteWrapper::update();
    Player::updateCounter();

    EffectManager::update(physicsTimestep);
    Pickups::update(physicsTimestep);
    POIs::update(physicsTimestep);
    UI::update(physicsTimestep);

    MapManager::update();

    go_next[64] = 0b1111111111111111;
    for (int i = 0; i < 64; ++i) {
        go_next[i] =
                (int(float((go_palette[i] & 0b1111100000000000) >> 11) * frac) << 11) |
                (int(float((go_palette[i] & 0b0000011111100000) >> 5) * frac) << 5) |
                (int(float(go_palette[i] & 0b0000000000011111) * frac));
    }
    RenderSystem::setPalette(go_next);

    if (go_f * 2.5f < -1.0f && Controls::getStatus().a.pressed()) {
        fsm.go(TitleState);
    }
}

void GameOver::draw()
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
    Soldier::drawAir();
    Enemy::drawAir();

    RenderSystem::print(55 - RenderSystem::getLineLength("GAME OVER")/2, 40, "GAME OVER", 64);

}
