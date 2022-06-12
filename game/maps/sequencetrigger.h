#ifndef SEQUENCETRIGGER_H
#define SEQUENCETRIGGER_H

#include "game/sequences.h"

struct SequenceTrigger : public Rect
{
    const SceneSequence * const linked_scene;

    enum SequenceID : int {
        Intro = 0,
        TutorialBoss = 1,
        TankBoss = 2,
        AcquireBoat = 3,
        FinalBoss = 4,

        LastID = 5
    } id;

    constexpr SequenceTrigger(int x, int y, int x2, int y2, const SceneSequence * const ref, SequenceID id) : Rect(x * 6, y * 6, (x2 - x) * 6, (y2 - y) * 6), linked_scene(ref), id(id) {}

    static bool checkForTriggers();

private:
    static const SequenceTrigger trigger_list[];
};


#endif // SEQUENCETRIGGER_H
