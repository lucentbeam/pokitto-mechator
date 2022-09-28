#include "title.h"

#include <cmath>

#include "core/rendersystem.h"
#include "core/palettes.h"
#include "core/controls.h"
#include "core/audiosystem.h"
#include "core/serialization.h"

#include "game/constants.h"

#include "game/sprites.h"
#include "game/states/game.h"
#include "game/utilities/helpers.h"
#include "game/utilities/mapmanager.h"
#include "game/player.h"

Title::TitleState Title::s_state = Title::Select;

int Title::select_index = 0;

GameStorageHeader Title::game_datas[3];

float Title::timer = 0.0f;
float Title::move_timer = 0.0f;


void Title::nextData()
{
    if (select_index == 2) return;
    ++select_index;
    AudioSystem::play(sfxSelect);
}

void Title::previousData()
{
    if (select_index == 0) return;
    --select_index;
    AudioSystem::play(sfxSelect);
}

void Title::selectData() {
    AudioSystem::play(sfxConfirm);

#ifdef DESKTOP_BUILD
#ifdef DEBUGS
    sprintf(GameVariables::savefile, "../data/mechator/save%d.dat", select_index + 1);
#else
    sprintf(GameVariables::savefile, "data/mechator/save%d.dat", select_index + 1);
#endif
#else
    sprintf(GameVariables::savefile, "/data/mechator/save%d.dat", select_index + 1);
#endif

    if (s_state == DataSelect && game_datas[select_index].elapsedMilliseconds > 5000) {
        GameStorage dat;
        Serialization::tryGet<GameStorage>(GameVariables::savefile, &dat);
        GameVariables::loadGame(dat);
        MapManager::loadMutables(GameVariables::savefile);
        Player::loadData();
    } else {
        GameVariables::loadGame(GameStorage());
        MapManager::resetMutables();
        Player::health(SoldierMode).setMax();
        Player::setPosition(SoldierMode, {6 * 6, 14 * 6}); // tutorial island
        Player::health(JeepMode).set(0);
        Player::health(BoatMode).set(0);
        Player::health(TankMode).set(0);
        Player::health(HelicopterMode).set(0);
    }

    goGame(true);
}

void Title::renderSaveDataInfo(int x, int y, GameStorageHeader &s, bool highlight, int idx)
{
    int col = highlight ? 10 : 8;
    if (s.elapsedMilliseconds < 5000) {
        RenderSystem::print(x - RenderSystem::getLineLength("------")/2, y, "------", col);
        return;
    }
    float time = float(s.elapsedMilliseconds) / 1000.0f;
    int hou = time / 3600.0f;
    int min = (time - hou * 3600) / 60.0f;
    int sec = time - hou * 3600 - min * 60;

    int p = idx;//s.percentageComplete();
    if (p < 0) p = 0;
    if (p > 100) p = 100;

    char buf[12];
    sprintf(buf, "FILE %d", p);
    int len = RenderSystem::getLineLength(buf);
    RenderSystem::print(x + 20 - len/2, y, buf, col);

    RenderSystem::print(x - 2, y, "-", col);

#ifdef DEBUGS
    sprintf(buf, "%02d:%02d", min, sec);
#else
    sprintf(buf, "%02d:%02d", hou, min);
#endif
    len = RenderSystem::getLineLength(buf);
    RenderSystem::print(x - 20 - len/2, y, buf, col);
}

void Title::go()
{
    select_index = 0;
    move_timer = 0.0f;
    s_state = Select;

#ifdef DESKTOP_BUILD
#ifdef DEBUGS
    Serialization::tryGet<GameStorageHeader>("../data/mechator/save1.dat", game_datas);
    Serialization::tryGet<GameStorageHeader>("../data/mechator/save2.dat", game_datas + 1);
    Serialization::tryGet<GameStorageHeader>("../data/mechator/save3.dat", game_datas + 2);
#else
    Serialization::tryGet<GameStorageHeader>("data/mechator/save1.dat", game_datas);
    Serialization::tryGet<GameStorageHeader>("data/mechator/save2.dat", game_datas + 1);
    Serialization::tryGet<GameStorageHeader>("data/mechator/save3.dat", game_datas + 2);
#endif
#else
    Serialization::tryGet<GameStorageHeader>("data/mechator/save1.dat", game_datas);
    Serialization::tryGet<GameStorageHeader>("data/mechator/save2.dat", game_datas + 1);
    Serialization::tryGet<GameStorageHeader>("data/mechator/save3.dat", game_datas + 2);
#endif

    RenderSystem::setPalette(default_palette);
    MapManager::resetMutables();
}

void Title::update(FSM &fsm)
{
    timer += physicsTimestep * 20.0f;
    move_timer += physicsTimestep * 5.0f;
    if (move_timer > 1.0f) move_timer = 1.0f;

    ControlStatus ctrl = Controls::getStatus();
    switch (s_state) {
    case Select:
        if (select_index > 0 && ctrl.up.pressed()) {
            select_index = 0;
            move_timer = 0.0f;
            AudioSystem::play(sfxSelect);
        } else if (select_index == 0 && ctrl.down.pressed()) {
            select_index = 1;
            move_timer = 0.0f;
            AudioSystem::play(sfxSelect);
        }
        if (ctrl.a.pressed() || ctrl.c.pressed()) {
            AudioSystem::play(sfxConfirm);
            s_state = select_index == 0 ? DataSelect : DataOverwrite;
            select_index = 0; // TOOD: first available
        }
        break;
    case DataSelect:
    case DataOverwrite:
        if (ctrl.b.pressed()) {
            AudioSystem::play(sfxCancel);
            select_index = s_state == DataSelect ? 0 : 1;
            s_state = Select;
            break;
        }
        if (ctrl.up.pressed()) previousData();
        else if (ctrl.down.pressed()) nextData();
        if (ctrl.a.pressed() || ctrl.c.pressed()) selectData();
        break;
    }

}

void Title::drawDataScreen()
{
    const int x = 14;
    const int y = 26;
    Helpers::drawNotchedRect(x, y, 110 - x * 2, 53, 0);
    Helpers::drawNotchedRect(x + 1, y + 1, 110 - x * 2 - 2, 9, 1);
    if (s_state == DataSelect) RenderSystem::print(x + 16, y + 2, "- LOAD GAME -", 9);
    else RenderSystem::print(x + 14, y + 2, "- OVERWRITE -", 9);

    renderSaveDataInfo(55, y + 16, game_datas[0], select_index == 0,1);
    renderSaveDataInfo(55, y + 28, game_datas[1], select_index == 1,2);
    renderSaveDataInfo(55, y + 40, game_datas[2], select_index == 2,3);
    RenderSystem::sprite(x + 2, y + 17 + 12 * select_index, ui_arrow_left, 0, true);
}

void Title::draw()
{
    RenderSystem::clear(50);
    RenderSystem::drawRect(0, 0, 7, 88, 0);
    RenderSystem::drawRect(103, 0, 7, 88, 0);
    const float spacing = 34.0f;
    float start = std::fmod(-timer, spacing);
    while (start < 88) {
        RenderSystem::drawRect(6, start, 98, 6, 0);
        start += spacing;
    }

    RenderSystem::print(93, 80, "v0.8", 6);
//    RenderSystem::print(6, 22, "(early access)", 6);
    Helpers::drawRLE(6, 6, title_text);

    if (s_state != Select) {
        drawDataScreen();
        return;
    }

    RenderSystem::drawRect(55 - 54 * move_timer, 32 + 4.0f + select_index * 11 - 2.0f * move_timer, 108 * move_timer, 3 * move_timer, 6);

    float frac = move_timer * 3.0f;
    frac *= frac;
    if (frac > 1.0f) frac = 1.0f;
    RenderSystem::print(66 - (select_index == 0 ? frac : 1 - frac) * 4, 32, "CONTINUE", select_index == 0 ? 10 : 8);
    RenderSystem::print(66 - (select_index == 1 ? frac : 1 - frac) * 4, 43, "NEW GAME", select_index == 1 ? 10 : 8);
    RenderSystem::sprite(49 + frac * 6.0f, 32 + 1 + select_index * 11, ui_arrow_left, 0, true);

    Helpers::drawRLE(0 , 58, title_jeep, 49);
    Helpers::drawRLE(58, 82, title_dirt);
    Helpers::drawRLE(65, 60, title_shot_1);
    Helpers::drawRLE(82, 64, title_shot_2);
}
