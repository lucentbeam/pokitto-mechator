#ifndef TITLE_H
#define TITLE_H

#include "core/utilities/babyfsm.h"

#include "game/variables.h"

class Title
{
    static enum TitleState {
        Select,
        DataSelect,
        DataOverwrite,
        ConfirmOverwrite
    } s_state;

    static int select_index;
    static GameStorageHeader game_datas[3];
    static float timer;
    static float move_timer;

    static void nextData();
    static void previousData();
    static void selectData();

    static void renderSaveDataInfo(int x, int y, GameStorageHeader &s, bool highlight);
    static void drawDataScreen();
public:
    static void go();

    static void update(FSM &fsm);

    static void draw();
};

#endif // TITLE_H
