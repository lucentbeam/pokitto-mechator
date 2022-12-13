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
#include "game/variables.h"

#include "game/ui/ui.h"

#ifndef DESKTOP_BUILD
#include "Pokitto.h"
#endif

UIOptions title_selection(3);

Title::TitleState Title::s_state = Title::Select;

GameStorageHeader Title::game_datas[3];

float Title::timer = 0.0f;
float Title::move_timer = 0.0f;

void Title::selectData() {
    AudioSystem::play(sfxConfirm);

#ifdef DESKTOP_BUILD
#ifdef DEBUGS
    sprintf(GameVariables::savefile, "../data/mechator/save%d.dat", title_selection.activeIndex() + 1);
#else
    sprintf(GameVariables::savefile, "data/mechator/save%d.dat", title_selection.activeIndex() + 1);
#endif
#else
    sprintf(GameVariables::savefile, "/data/mechator/save%d.dat", title_selection.activeIndex() + 1);
#endif

    Player::reset();
    if (s_state == DataSelect && game_datas[title_selection.activeIndex()].elapsedMilliseconds > 5000) {
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
#if DEBUGS
    for(int i = 0; i < Blueprints::LastIdxBP; ++i) {
        GameVariables::acquireBlueprint(Blueprints(i));
//        GameVariables::unlockBlueprint(Blueprints(i));
    }
#endif
    Player::updateOwnedWeapons();

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
    title_selection.setAvailableCount(3);
    title_selection.setSelection(0);
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

void Title::updateTimer(bool reset) {
    timer += physicsTimestep * 20.0f;
    if (reset) {
        move_timer = 0.0f;
        return;
    }
    move_timer += physicsTimestep * 5.0f;
    if (move_timer > 1.0f) move_timer = 1.0f;
}

void Title::update(FSM &fsm)
{
    ControlStatus ctrl = Controls::getStatus();
    updateTimer(title_selection.update(ctrl.down.pressed(), ctrl.up.pressed()) != 0);
    switch (s_state) {
    case Select:
        if (ctrl.a.pressed() || ctrl.c.pressed()) {
            AudioSystem::play(sfxConfirm);
            s_state = title_selection.activeIndex() == 0 ? DataSelect : title_selection.activeIndex() == 1 ? DataOverwrite : Options;
            if (s_state == Options) { title_selection.setAvailableCount(4); title_selection.setSelection(0); }
            title_selection.setSelection(0);
        }
        break;
    case DataSelect:
    case DataOverwrite:
        if (ctrl.b.pressed()) {
            AudioSystem::play(sfxCancel);
            title_selection.setSelection(s_state == DataSelect ? 0 : 1);
            s_state = Select;
            break;
        }
        if (ctrl.a.pressed() || ctrl.c.pressed()) selectData();
        break;
    case Options:
        if (updateOptions(title_selection, ctrl)) {
            GameOptions::save();
            s_state = Select;
            title_selection.setAvailableCount(3);
            title_selection.setSelection(2);
        }
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

    renderSaveDataInfo(55, y + 16, game_datas[0], title_selection.activeIndex() == 0,1);
    renderSaveDataInfo(55, y + 28, game_datas[1], title_selection.activeIndex() == 1,2);
    renderSaveDataInfo(55, y + 40, game_datas[2], title_selection.activeIndex() == 2,3);
    RenderSystem::sprite(x + 2, y + 17 + 12 * title_selection.activeIndex(), ui_arrow_left, 0, true);
}

bool Title::updateOptions(const UIOptions &opts, const ControlStatus &ctrl)
{
    if (ctrl.b.pressed()) {
        AudioSystem::play(sfxCancel);
        return true;
    }
    switch(opts.activeIndex()) {
    case 0:
        if (ctrl.right.downEvery(1, 5)) GameOptions::setVolumeFrac(GameOptions::volumeFrac() + 0.125f);
        if (ctrl.left.downEvery(1, 5)) GameOptions::setVolumeFrac(GameOptions::volumeFrac() - 0.125f);
        break;
    case 1:
        if (ctrl.a.pressed() || ctrl.right.pressed() || ctrl.left.pressed()) GameOptions::setMusicOn(!GameOptions::musicOn());
        break;
    case 2:
        if (ctrl.a.pressed() || ctrl.right.pressed() || ctrl.left.pressed()) GameOptions::setSfxOn(!GameOptions::sfxOn());
        break;
    case 3:
        if (ctrl.a.pressed()) {
#ifndef DESKTOP_BUILD
            Pokitto::Core::jumpToLoader();
#endif
        }
        break;
    }
    return false;
}

void Title::drawOptionScreen(const UIOptions &opt, int yoffset)
{
    const int x = 14;
    int top = 26 + yoffset;
//    Helpers::drawNotchedRect(x, y, 110 - x * 2, 53, 0);
//    Helpers::drawNotchedRect(x + 1, y + 1, 110 - x * 2 - 2, 9, 1);
    RenderSystem::print(x + 26, top, "OPTIONS", 10);
    float frac = move_timer * 3.0f;
    frac *= frac;
    if (frac > 1.0f) frac = 1.0f;
    top += 12;
    const int spacing = 10;
    opt.foreach([&](int8_t idx, bool active) {
        if (active) {
            RenderSystem::drawRect(55 - 54 * move_timer, top + 4.0f + idx * spacing - 2.0f * move_timer, 108 * move_timer, 3 * move_timer, 6);
            RenderSystem::sprite(6, top + 1 + spacing * idx, ui_arrow_left, 0, true);
        }
        const char * txt[] = {"VOLUME", "MUSIC", "SFX", "QUIT TO BOOTLOADER"};
        RenderSystem::print(14, top + spacing * idx, txt[idx], active ? 10 : 8);
    });
    int v = GameOptions::volumeFrac() * 8;
    for(int i = 1; i < 9; ++i) {
        if (i <= v) {
            RenderSystem::drawRect2(57 + 5 * (i - 1), top + 1, 3, 5, opt.activeIndex() == 0 ? 10 : 8);
        } else {
            RenderSystem::drawRect2(57 + 5 * (i - 1), top + 3, 3, 1, opt.activeIndex() == 0 ? 9 : 8);
        }
    }
    RenderSystem::print(96 - RenderSystem::getLineLength(GameOptions::musicOn() ? "ON" : "OFF"), top + spacing, GameOptions::musicOn() ? "ON" : "OFF", title_selection.activeIndex() == 1 ? 10 : 8);
    RenderSystem::print(96 - RenderSystem::getLineLength(GameOptions::sfxOn() ? "ON" : "OFF"), top + spacing * 2, GameOptions::sfxOn() ? "ON" : "OFF", title_selection.activeIndex() == 2 ? 10 : 8);
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

    RenderSystem::print(93, 80, "v1.0", 6);
    Helpers::drawRLE(6, 6, title_text);

    switch (s_state) {
    case ConfirmOverwrite:
    case DataSelect:
    case DataOverwrite:
        drawDataScreen();
        return;
    case Options:
        drawOptionScreen(title_selection);
        return;
    }

    float frac = move_timer * 3.0f;
    frac *= frac;
    if (frac > 1.0f) frac = 1.0f;

    title_selection.foreach([&](int8_t idx, bool active) {
        const int top = 28;
        const int spacing = 10;
        if (active) {
            RenderSystem::drawRect(55 - 54 * move_timer, top + 4.0f + idx * spacing - 2.0f * move_timer, 108 * move_timer, 3 * move_timer, 6);
            RenderSystem::sprite(49 + frac * 6.0f, top + 1 + idx * spacing, ui_arrow_left, 0, true);
        }
        const char * txt[] = {"CONTINUE", "NEW GAME", "OPTIONS"};
        RenderSystem::print(66 - (active ? frac : 0) * 4, top + idx * spacing, txt[idx], active ? 10 : 8);
    });

    Helpers::drawRLE(0 , 58, title_jeep, 49);
    Helpers::drawRLE(58, 82, title_dirt);
    Helpers::drawRLE(63, 62, title_shot_1);
    Helpers::drawRLE(82, 65, title_shot_2);
}
