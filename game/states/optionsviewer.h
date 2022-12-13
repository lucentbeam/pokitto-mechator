#ifndef OPTIONSVIEWER_H
#define OPTIONSVIEWER_H

#include "core/utilities/babyfsm.h"

class OptionsViewer
{
public:
    static void go();
    static void update(FSM &fsm);
    static void draw();
};

#endif // OPTIONSVIEWER_H
