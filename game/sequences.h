#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "core/utilities/tween.h"

#include "game/player.h"
#include "game/utilities/sceneobjects.h"
#include "game/entities/enemy.h"
#include "game/states/eventscene.h"
#include "game/states/game.h"

#include "game/entities/barracks.h"

const SceneWait wait60 = SceneWait(60);

extern Tween toLand;

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
    {SceneSequence::Wait, &wait60 },
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
    h->setMaxLife(18);
    UI::showBoss(h->getLifePtr());
    // set door behind player (218) on (51,19) through (51, 22)
    MapManager::setTileAt(51 * 6 + 3, 19 * 6 + 3, 218);
    MapManager::setTileAt(51 * 6 + 3, 20 * 6 + 3, 218);
    MapManager::setTileAt(51 * 6 + 3, 21 * 6 + 3, 218);
    MapManager::setTileAt(51 * 6 + 3, 22 * 6 + 3, 218);
    registerCallback({h->getLifePtr(),  Barracks::getBarracksAt({55, 11})->getLifePtr(), Barracks::getBarracksAt({69, 20})->getLifePtr()}, [](){
        // release door as grass (86)
        MapManager::setTileAt(51 * 6 + 3, 19 * 6 + 3, 86);
        MapManager::setTileAt(51 * 6 + 3, 20 * 6 + 3, 86);
        MapManager::setTileAt(51 * 6 + 3, 21 * 6 + 3, 86);
        MapManager::setTileAt(51 * 6 + 3, 22 * 6 + 3, 86);
    });
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
const SceneMoveCam cam_return = SceneMoveCam(cameraCutsceneSpeed);

const SceneMoveCam tb_m0 = SceneMoveCam(cameraCutsceneSpeed, {65, 15});

const SceneDialogue tb_dlog0 = SceneDialogue("INFILTRATOR!",nullptr, true, false);
const SceneDialogue tb_dlog1 = SceneDialogue("YOU WILL GET", "NO FURTHER.", true, false);
const SceneDialogue tb_dlog2 = SceneDialogue("NOR FARTHER,         ", "FOR THAT MATTER.", true, true);

const SceneSequence tutorial_boss_scene[] = {
    {SceneSequence::DoFunction, &tb_f0},
    {SceneSequence::MoveCamera, &tb_m0 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &tb_dlog0 },
    {SceneSequence::ShowDialogue, &tb_dlog1 },
    {SceneSequence::ShowDialogue, &tb_dlog2 },
    {SceneSequence::MoveCamera, &cam_return },
    {SceneSequence::End, &cam_release}
};

const SceneMoveCam tt_m0 = SceneMoveCam(cameraCutsceneSpeed, {27, 99});
const SceneMoveCam tt_m1 = SceneMoveCam(cameraCutsceneSpeed, {19, 94});

const SceneDialogue tt_dlog0 = SceneDialogue("IT IS INFILTRATOR!",nullptr, true, false);
const SceneDialogue tt_dlog1 = SceneDialogue("YOU HAVE COME FOR", "TANK BLUEPRINTS, YES?", true, false);
const SceneDialogue tt_dlog2 = SceneDialogue("TOO BAD.", nullptr, true, false);
const SceneDialogue tt_dlog3 = SceneDialogue("THEY HAVE ALREADY", "BEEN RELOCATED.", true, false);
const SceneDialogue tt_dlog4 = SceneDialogue("NOW TO RELOCATE YOU.", nullptr, true, true);

const SceneDialogue tt_dlog5 = SceneDialogue("      ??????", nullptr, true, true);

const SceneDialogue tt_dlog6 = SceneDialogue("TO...    ", "    ...umm...", true, true);
const SceneDialogue tt_dlog7 = SceneDialogue("THE AFTERLIFE!", nullptr, true, true);

const SceneDialogue tt_dlog8 = SceneDialogue("NICE SAVE.", nullptr, true, true);

const SceneDialogue tt_dlog9 = SceneDialogue("THANK YOU.", nullptr, true, true);

const SceneSequence tank_boss_scene[] = {
//    {SceneSequence::DoFunction, &tb_f0},
    {SceneSequence::MoveCamera, &tt_m0 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &tt_dlog0 },
    {SceneSequence::ShowDialogue, &tt_dlog1 },
    {SceneSequence::ShowDialogue, &tt_dlog2 },
    {SceneSequence::ShowDialogue, &tt_dlog3 },
    {SceneSequence::ShowDialogue, &tt_dlog4 },
    {SceneSequence::MoveCamera, &tt_m1 },
    {SceneSequence::ShowDialogue, &tt_dlog5 },
    {SceneSequence::MoveCamera, &tt_m0 },
    {SceneSequence::ShowDialogue, &tt_dlog6 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &tt_dlog7 },
    {SceneSequence::MoveCamera, &tt_m1 },
    {SceneSequence::ShowDialogue, &tt_dlog8 },
    {SceneSequence::MoveCamera, &tt_m0 },
    {SceneSequence::ShowDialogue, &tt_dlog9 },
    {SceneSequence::MoveCamera, &cam_return },
    {SceneSequence::End, &cam_release}
};

const SceneMoveCam by_m0 = SceneMoveCam(cameraCutsceneSpeed, {165, 206});

const SceneDialogue by_dlog0 = SceneDialogue("Soldier, come in.", nullptr, false, false);
//const SceneDialogue dlog1 = SceneDialogue("This is central.", nullptr, false, false);
const SceneDialogue by_dlog2 = SceneDialogue("Great job making it", "to the boat yard.", false, true);

const SceneDialogue by_dlog3 = SceneDialogue("But it looks like","it's out of order.", false, true);
const SceneDialogue by_dlog4 = SceneDialogue("Get the blueprint","and hit that switch.", false, false);
const SceneDialogue by_dlog5 = SceneDialogue("That will lower a","bridge in the north.", false, false);
const SceneDialogue by_dlog6 = SceneDialogue("We've detected a","boat yard up there.", false, false);

const SceneSequence boatyard_scene[] = {
    {SceneSequence::ShowDialogue, &by_dlog0 },
    {SceneSequence::ShowDialogue, &dlog1 },
    {SceneSequence::ShowDialogue, &by_dlog2 },

    {SceneSequence::MoveCamera, &by_m0 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &by_dlog3 },
    {SceneSequence::ShowDialogue, &by_dlog4 },
    {SceneSequence::ShowDialogue, &by_dlog5 },
    {SceneSequence::ShowDialogue, &by_dlog6 },
    {SceneSequence::ShowDialogue, &dlog6 },

    {SceneSequence::MoveCamera, &cam_return },
    {SceneSequence::End, &cam_release}
};
#endif // SEQUENCES_H
