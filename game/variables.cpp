#include "variables.h"

#include "game/maps/sequencetrigger.h"
#include "game/enums.h"

uint8_t GameVariables::hackingKitCount = 0;
uint8_t GameVariables::keyACount = 0;
uint8_t GameVariables::keyBCount = 0;
uint8_t GameVariables::keyCCount = 0;
#ifdef DEBUGS
uint16_t GameVariables::dollarCount = 100;
#else
uint16_t GameVariables::dollarCount = 50;
#endif
uint16_t GameVariables::acquiredBlueprints = 0;
uint16_t GameVariables::unlockedBlueprints = 0;
uint8_t GameVariables::questStatus = 0;
bool GameVariables::visitedEvents[SequenceTrigger::LastID] = { false };

void GameVariables::changeDollars(int16_t delta)
{
    dollarCount += std::fmax(delta, -dollarCount);
}

void GameVariables::changeHackingKits(int8_t delta)
{
    hackingKitCount += delta;
}

void GameVariables::changeKeysA(int8_t delta)
{
    keyACount += delta;
}

void GameVariables::changeKeysB(int8_t delta)
{
    keyBCount += delta;
}

void GameVariables::changeKeysC(int8_t delta)
{
    keyCCount += delta;
}

bool GameVariables::hasBlueprint(int bp)
{
    return (acquiredBlueprints & (1 << bp)) > 0;
}

void GameVariables::acquireBlueprint(int bp)
{
    acquiredBlueprints = acquiredBlueprints | (1 << bp);
}

bool GameVariables::hasBlueprintUnlocked(int bp)
{
    return (unlockedBlueprints & (1 << bp)) > 0;
}

void GameVariables::unlockBlueprint(int bp)
{
    unlockedBlueprints = unlockedBlueprints | (1 << bp);
}

bool GameVariables::hasBlueprintToUnlock(int bp)
{
    return hasBlueprint(bp) && !hasBlueprintUnlocked(bp);
}

bool GameVariables::hasUnusedBlueprints()
{
    return acquiredBlueprints != unlockedBlueprints;
}

uint8_t GameVariables::keysA() { return keyACount; }

uint8_t GameVariables::keysB() { return keyBCount; }

uint8_t GameVariables::keysC() { return keyCCount; }

uint8_t GameVariables::hackingKits() { return hackingKitCount; }

uint16_t GameVariables::dollars() { return dollarCount; }

void GameVariables::visitEvent(int index) { visitedEvents[index] = true; }

bool GameVariables::eventVisited(int index) { return visitedEvents[index]; }

void GameVariables::setQuestStatus(uint8_t q)
{
    questStatus = q;
}

Vec2i GameVariables::getGoal()
{
    switch (QuestStatus(questStatus)) {
    case QuestIntro:
        return Vec2i(26, 48);
    case QuestTank:
        return Vec2i(26, 102);
    case QuestBoat:
        return Vec2i(160, 202);
    case QuestBuildBoat:
        return Vec2i(209, 32);
    case QuestHeli:
        return Vec2i(-20, 210);
    case QuestFinal:
        return Vec2i(103, 147);
    default:
        return Vec2i(-500, -500);
    }
}
