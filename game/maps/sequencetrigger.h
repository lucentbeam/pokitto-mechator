#ifndef SEQUENCETRIGGER_H
#define SEQUENCETRIGGER_H

#include "game/sequences.h"

struct SequenceTrigger : public Rect
{
    const SceneSequence * const linked_scene;

    SequenceID id;

    constexpr SequenceTrigger(int x, int y, int x2, int y2, const SceneSequence * const ref, SequenceID id) : Rect(x * 6, y * 6, (x2 - x) * 6, (y2 - y) * 6), linked_scene(ref), id(id) {}

    static bool checkForTriggers();

    static bool checkFinalSequence();

private:
    static const SequenceTrigger trigger_list[];
};


#endif // SEQUENCETRIGGER_H
