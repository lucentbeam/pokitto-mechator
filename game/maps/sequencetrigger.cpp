#include "sequencetrigger.h"

#include "game/player.h"
#include "game/states/eventscene.h"
#include "game/variables.h"

const SequenceTrigger SequenceTrigger::trigger_list[] = {
    {4, 14, 9, 19, intro_scene, SequenceTrigger::Intro},
    {54, 18, 55, 23, tutorial_boss_scene, SequenceTrigger::TutorialBoss},
    {25, 105, 30, 107, tank_boss_scene, SequenceTrigger::TankBoss},
    {147, 195, 149, 205, boatyard_scene, SequenceTrigger::AcquireBoat},
};

bool SequenceTrigger::checkForTriggers()
{
    for(const SequenceTrigger &st : trigger_list) {
        if (!GameVariables::eventVisited(st.id) && st.contains(Player::position())) {
            GameVariables::visitEvent(st.id);
            EventScene::startScene(st.linked_scene);
            return true;
        }
    }
    return false;
}
