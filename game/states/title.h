#ifndef TITLE_H
#define TITLE_H

#include "core/utilities/babyfsm.h"

struct GameStorageHeader;
struct ControlStatus;
class UIOptions;

class Title
{
    static enum TitleState {
        Select,
        DataSelect,
        DataOverwrite,
        ConfirmOverwrite,
        Options
    } s_state;

    static GameStorageHeader game_datas[3];
    static float timer;
    static float move_timer;

    static void selectData();

    static void renderSaveDataInfo(int x, int y, GameStorageHeader &s, bool highlight, int idx);
    static void drawDataScreen();

public:
    static void updateTimer(bool reset);
    static bool updateOptions(const UIOptions &opts, const ControlStatus &ctrl);
    static void drawOptionScreen(const UIOptions &opt, int yoffset = 0);

    static void go();

    static void update(FSM &fsm);

    static void draw();
};

#endif // TITLE_H
