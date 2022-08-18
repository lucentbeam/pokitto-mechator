#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "core/utilities/babyfsm.h"

class GameOver
{
public:
    static void go();
    static void update(FSM &fsm);
    static void draw();
};

#endif // GAMEOVER_H
