#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "core/utilities/tween.h"

#include "game/player.h"
#include "game/utilities/sceneobjects.h"
#include "game/entities/enemy.h"
#include "game/states/eventscene.h"

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
const SceneDialogue dlog0 = SceneDialogue("Soldier!       ","Can you hear me?", false, false);
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

const SceneFunc tb_f0 = SceneFunc([](){
    EnemyHelicopter * h = Enemy::createHelicopter({80 * 6, 14 * 6});
    EventScene::setTextSpeed(baseTextSpeedLPS / 2.0f);
    EventScene::registerUpdate([h](){
        h->tick();
        Vec2f d = Camera::center() - h->getSteering().pos();
        float l = d.length();
        if (l > 0) {
            d /= l;
        }
        if (l > 5) h->getSteering().update(physicsTimestep, d.x(), d.y());
        return false;
    });
    return true;
});
const SceneFunc cam_release = SceneFunc([](){ Camera::stopMovement(); return true; });

const SceneMoveCam tb_m0 = SceneMoveCam(12.0f, {65, 15});
const SceneMoveCam tb_m1 = SceneMoveCam(12.0f);

const SceneDialogue tb_dlog0 = SceneDialogue("INFILTRATOR.",nullptr, true, false);
const SceneDialogue tb_dlog1 = SceneDialogue("YOU WILL GET", "NO FURTHER.", true, false);
const SceneDialogue tb_dlog2 = SceneDialogue("NOR FARTHER,         ", "FOR THAT MATTER.", true, true);

const SceneSequence tutorial_boss_scene[] = {
    {SceneSequence::DoFunction, &tb_f0},
    {SceneSequence::MoveCamera, &tb_m0 },
    {SceneSequence::Wait, &wait0 },
    {SceneSequence::ShowDialogue, &tb_dlog0 },
    {SceneSequence::ShowDialogue, &tb_dlog1 },
    {SceneSequence::ShowDialogue, &tb_dlog2 },
    {SceneSequence::MoveCamera, &tb_m1 },
    {SceneSequence::End, &cam_release}
};

#endif // SEQUENCES_H
