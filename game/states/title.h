#ifndef TITLE_H
#define TITLE_H

#include "core/utilities/babyfsm.h"

class Title
{
    static enum TitleState {
        Select,
        DataSelect,
    } s_state;

    static int select_index;
    static int data_index;
    static bool has_data[3];
public:
    static void go();

    static void update(FSM &fsm);

    static void draw();
};

#endif // TITLE_H
