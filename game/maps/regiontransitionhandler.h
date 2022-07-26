#ifndef REGIONTRANSITIONHANDLER_H
#define REGIONTRANSITIONHANDLER_H

#include "game/maps/alertregion.h"

class RegionTransitionHandler
{
    enum CurrentStatus {
        None = 0,
        Overworld = 1,
        Peninsula = 2,
        Boss = 3,
        FinalBoss = 4,
    };

    struct State {
        CurrentStatus status = None;
        CurrentStatus previous = None;
        bool in_boss = false;
        int transition_trackers[5] = {0, 0, 0, 0, 0};
    };

    static State s_state;

    static void updateBoss();
    static void updateOverworld();
    static void updatePeninsula();
    static void updateFinalBoss();
public:

    static void goRegion(RegionNames name);
    static void goBoss(bool final = false);
    static void leaveBoss();

    static void update();

    static bool atBoss() { return s_state.in_boss; }
};

#endif // REGIONTRANSITIONHANDLER_H
