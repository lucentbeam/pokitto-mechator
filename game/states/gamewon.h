#ifndef GAMEWON_H
#define GAMEWON_H

#include "core/utilities/babyfsm.h"

namespace EndCredits {
    enum State {
        Fade,
        Credits,
        Fin
    };

    extern State credits_state;
}

void goGameWonState();

void updateGameWonState(FSM &fsm);

void drawGameWonState();

#endif // GAMEWON_H
