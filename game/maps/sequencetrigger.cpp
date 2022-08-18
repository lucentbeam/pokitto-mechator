#include "sequencetrigger.h"

#include "game/player.h"
#include "game/states/eventscene.h"
#include "game/variables.h"

const SequenceTrigger SequenceTrigger::trigger_list[] = {
    {4, 14, 9, 19, intro_scene, SequenceID::Intro},
    {54, 18, 55, 23, tutorial_boss_scene, SequenceID::TutorialBoss},
    {25, 105, 30, 107, tank_boss_scene, SequenceID::TankBoss},
    {147, 195, 149, 205, boatyard_scene, SequenceID::AcquireBoat},

    {91,  134, 125, 152,  finalboss_scene,  SequenceID::FinalBoss},
    {106, 150, 126, 168,  finalboss_scene,  SequenceID::FinalBoss},
    {123, 156, 142, 178,  finalboss_scene,  SequenceID::FinalBoss},
    {15, 21, 19, 23, exit_tut_island, SequenceID::ExitTutorial}
};

const SequenceTrigger tank_enter = SequenceTrigger(0, 0, 0, 0, acquiredtank_scene, SequenceID::EnterTank);
const SequenceTrigger boat_enter = SequenceTrigger(0, 0, 0, 0, acquiredboat_scene, SequenceID::EnterBoat);
const SequenceTrigger helicopter_takeoff = SequenceTrigger(0, 0, 0, 0, acquiredheli_scene, SequenceID::EnterHelicopter);

bool final_sequence_run = false;

bool SequenceTrigger::checkForTriggers()
{
    for(const SequenceTrigger &st : trigger_list) {
        if (!GameVariables::eventVisited(st.id) && st.contains(Player::position())) {
            GameVariables::visitEvent(st.id);
            EventScene::startScene(st.linked_scene);
            return true;
        }
    }
    if (!GameVariables::eventVisited(tank_enter.id) && Player::mode() == PlayerMode::TankMode) {
        GameVariables::visitEvent(tank_enter.id);
        EventScene::startScene(tank_enter.linked_scene);
        return true;
    }
    if (!GameVariables::eventVisited(boat_enter.id) && Player::mode() == PlayerMode::BoatMode) {
        GameVariables::visitEvent(boat_enter.id);
        EventScene::startScene(boat_enter.linked_scene);
        return true;
    }
    if (!GameVariables::eventVisited(helicopter_takeoff.id) && Helicopter::active()) {
        GameVariables::visitEvent(helicopter_takeoff.id);
        EventScene::startScene(helicopter_takeoff.linked_scene);
        return true;
    }
    return false;
}

bool SequenceTrigger::checkFinalSequence()
{
    if (final_sequence_run) return true;
    final_sequence_run = true;
    EventScene::startScene(win_scene);
    return false;
}

bool SequenceTrigger::tutorialComplete()
{
    return GameVariables::eventVisited((trigger_list + 7)->id);
}
