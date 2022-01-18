#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "core/utilities/tween.h"

#include "game/player.h"
#include "game/utilities/sceneobjects.h"

extern Tween toLand;
const SceneWait wait0 = SceneWait(60);
const SceneFunc func0 = SceneFunc([](){ toLand.reset(); return true; });
const SceneFunc func1 = SceneFunc([](){
    int x = toLand.getInterpolationInt(-20, playerStartTileX * 6 + 3);
    int z = toLand.getInterpolationInt(60, 0);
    Soldier::setPosition({x, Soldier::position().y()});
    Soldier::setZ(z);
    return toLand.done();
});
const SceneDialogue dlog0 = SceneDialogue("Mechator!       ","Can you hear me?", false, false);
const SceneDialogue dlog1 = SceneDialogue("This is central.", nullptr, false, false);
const SceneDialogue dlog2 = SceneDialogue("You're in enemy","territory now.", false, false);
const SceneDialogue dlog3 = SceneDialogue("Find their shops!       ","Hack them. Use them.", false, false);
const SceneDialogue dlog4 = SceneDialogue("You can build your","own weapons...", false, false);
const SceneDialogue dlog5 = SceneDialogue("...and infiltrate!",nullptr, false, false);
const SceneDialogue dlog6 = SceneDialogue("Good luck.        ", "Central out.", false, true);

const SceneSequence intro_scene[] = {
    {SceneSequence::DoFunction, &func0},
    {SceneSequence::DoFunction, &func1},
    {SceneSequence::Wait, &wait0 },
    {SceneSequence::ShowDialogue, &dlog0 },
    {SceneSequence::ShowDialogue, &dlog1 },
    {SceneSequence::ShowDialogue, &dlog2 },
    {SceneSequence::ShowDialogue, &dlog3 },
    {SceneSequence::ShowDialogue, &dlog4 },
    {SceneSequence::ShowDialogue, &dlog5 },
    {SceneSequence::ShowDialogue, &dlog6 },
    {SceneSequence::End, nullptr}
};

#endif // SEQUENCES_H
