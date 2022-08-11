#include "variables.h"

#include "core/serialization.h"
#include "game/maps/sequencetrigger.h"

bool GameVariables::gameWon = false;

GameStorage GameVariables::s_data;

char GameVariables::savefile[30] = "/data/mechator/save1.dat";

void GameVariables::changeDollars(int16_t delta)
{
    s_data.dollarCount += std::fmax(delta, -s_data.dollarCount);
}

void GameVariables::changeHackingKits(int8_t delta)
{
    s_data.hackingKitCount += delta;
}

void GameVariables::changeKeysA(int8_t delta)
{
    s_data.keyACount += delta;
}

void GameVariables::changeKeysB(int8_t delta)
{
    s_data.keyBCount += delta;
}

void GameVariables::changeKeysC(int8_t delta)
{
    s_data.keyCCount += delta;
}

bool GameVariables::hasBlueprint(int bp)
{
    return (s_data.acquiredBlueprints & (1 << bp)) > 0;
}

void GameVariables::acquireBlueprint(int bp)
{
    s_data.acquiredBlueprints = s_data.acquiredBlueprints | (1 << bp);
}

bool GameVariables::hasBlueprintUnlocked(int bp)
{
    return (s_data.unlockedBlueprints & (1 << bp)) > 0;
}

void GameVariables::unlockBlueprint(int bp)
{
    s_data.unlockedBlueprints = s_data.unlockedBlueprints | (1 << bp);
}

bool GameVariables::hasBlueprintToUnlock(int bp)
{
    return hasBlueprint(bp) && !hasBlueprintUnlocked(bp);
}

bool GameVariables::hasUnusedBlueprints()
{
    return s_data.acquiredBlueprints != s_data.unlockedBlueprints;
}

uint8_t GameVariables::keysA() { return s_data.keyACount; }

uint8_t GameVariables::keysB() { return s_data.keyBCount; }

uint8_t GameVariables::keysC() { return s_data.keyCCount; }

uint8_t GameVariables::hackingKits() { return s_data.hackingKitCount; }

uint16_t GameVariables::dollars() { return s_data.dollarCount; }

void GameVariables::visitEvent(int index) { s_data.visitedEvents[index] = true; }

bool GameVariables::eventVisited(int index) { return s_data.visitedEvents[index]; }

void GameVariables::setQuestStatus(uint8_t q)
{
    s_data.questStatus = q;
}

Vec2i GameVariables::getGoal()
{
    switch (QuestStatus(s_data.questStatus)) {
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

void GameVariables::setGameWon()
{
    gameWon = true;
}

bool GameVariables::getGameWon()
{
    return gameWon;
}

void GameVariables::updateTime(int ms)
{
    s_data.elapsedMilliseconds += ms;
}

void GameVariables::saveGame()
{
    Serialization::tryStore<GameStorage>(savefile, &s_data);
}

void GameVariables::loadGame(GameStorage s)
{
    s_data = s;
}
