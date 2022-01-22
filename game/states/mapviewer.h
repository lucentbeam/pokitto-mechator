#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include "core/utilities/babyfsm.h"

class MapViewer
{
public:
    static void go();
    static void update(FSM &fsm);
    static void draw();
};

#endif // MAPVIEWER_H
