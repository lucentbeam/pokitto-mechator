#include "gamewon.h"

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

const float spacing = 13.0f;
const float gap = 11.0f;
const float credit_space = 80.0f;

static float f = 2.0f;
const uint16_t * palette;
uint16_t next[64];

const char * credits[] = {
    "- scenario -",
    "- design -",
    "- music -",
    "- programmer -",
    "- special thanks -",
    "",
    "fin."
};

static char duration_buf[9];

EndCredits::State EndCredits::credits_state = EndCredits::Fade;

void goGameWonState()
{
    float time = float(GameVariables::getData()->header.elapsedMilliseconds) / 1000.0f;
    int hou = time / 3600.0f;
    int min = (time - hou * 3600) / 60.0f;
    int sec = time - hou * 3600 - min * 60;
    sprintf(duration_buf, "%02d:%02d:%02d", hou, min, sec);

    EffectManager::createExplosionBig(Player::position() - Vec2i(7, 7));
    AudioSystem::play(sfxExplosionBig);
    palette = RenderSystem::getPalette();
    AudioSystem::playSong(musNone);
}

void updateGameWonState(FSM &fsm)
{
    float frac = f;

    switch (EndCredits::credits_state) {
    case EndCredits::Fade:
        f -= physicsTimestep / 1.3f;
        if (f < 0) frac = 0;
        if (f > 1.0) frac = 1.0;
        SpriteWrapper::update();
        Player::updateCounter();

        EffectManager::update(physicsTimestep);
        Pickups::update(physicsTimestep);
        POIs::update(physicsTimestep);
        UI::update(physicsTimestep);

        MapManager::update();


        for (int i = 0; i < 64; ++i) {
            next[i] =
                    (int(float((palette[i] & 0b1111100000000000) >> 11) * frac) << 11) |
                    (int(float((palette[i] & 0b0000011111100000) >> 5) * frac) << 5) |
                    (int(float(palette[i] & 0b0000000000011111) * frac));
        }

        if (f * 2.5f < -1.0f) {
            EndCredits::credits_state = EndCredits::Credits;
            next[1] = 0b0111101111101111;
            next[2] = 0b1111111111111111;
            f = 88.0f;
        }
        RenderSystem::setPalette(next);

        break;
    case EndCredits::Credits:
        f -= 25.0f * physicsTimestep;
        if (f < (88 - 6 * credit_space - 48)) {
            f = 88 - 6 * credit_space - 48;
            EndCredits::credits_state = EndCredits::Fin;
        }
        break;
    case EndCredits::Fin:

        break;
    }
}

void drawGameWonState()
{

    float pos;
    switch (EndCredits::credits_state) {
    case EndCredits::Fade:
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



        break;
    case EndCredits::Credits:
        RenderSystem::clear(0);
        for(int i = 0; i < 7; ++i) {
            pos = f + i * credit_space;
            if (pos > -12 && pos <  176) RenderSystem::print(55 - RenderSystem::getLineLength(credits[i])/2, pos, credits[i], 1);
            if (i < 5) {
                pos += spacing;
                if (pos > -12 && pos <  176) RenderSystem::print(55 - RenderSystem::getLineLength("lucentbeam")/2, pos, "lucentbeam", 2);
                if (i == 4) {
                    pos += gap;
                    if (pos > -12 && pos <  176) RenderSystem::print(55 - RenderSystem::getLineLength("Retr0rob")/2, pos, "Retr0rob", 2);
                    pos += gap;
                    if (pos > -12 && pos <  176) RenderSystem::print(55 - RenderSystem::getLineLength("pokitto community")/2, pos, "pokitto community", 2);
                }
            }
        }

        break;
    case EndCredits::Fin:
        RenderSystem::clear(0);
        RenderSystem::print(55 - RenderSystem::getLineLength("fin")/2, f + 6 * credit_space, "fin.", 2);
        Helpers::printHorizontallyCentered(55, 76, duration_buf, 1);

        break;
    }

}

