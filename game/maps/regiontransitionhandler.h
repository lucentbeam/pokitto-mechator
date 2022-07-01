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
    };

    struct State {
        CurrentStatus status = None;
        CurrentStatus previous = None;
        bool in_boss = false;
        int transition_trackers[4] = {0, 0, 0, 0};
    };

    static State s_state;

    static void updateBoss();
    static void updateOverworld();
    static void updatePeninsula();
    static void updateFinalBoss();
public:

    static void goRegion(RegionNames name);
    static void goBoss();
    static void leaveBoss();

    static void update();
};

#endif // REGIONTRANSITIONHANDLER_H
