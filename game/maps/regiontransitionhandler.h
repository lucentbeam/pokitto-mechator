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
        Swamp = 5,
        NorthBase = 6,
        Canyons = 7,
        Boating = 8
    };

    struct State {
        CurrentStatus status = None;
        CurrentStatus previous = None;
        bool in_boss = false;
        int transition_trackers[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    };

    static State s_state;

    static void updateBoss();
    static void updateBoat();
    static void updateOverworld();
    static void updatePeninsula();
    static void updateFinalBoss();
    static void updateSwamp();
    static void updateNorthBase();
    static void updateCanyons();
public:

    static void goRegion(RegionNames name);
    static void goBoss(bool final = false);
    static void leaveBoss();

    static void update();

    static bool atBoss() { return s_state.in_boss; }

    static void clear();
};

#endif // REGIONTRANSITIONHANDLER_H
