#include "variables.h"


uint8_t GameVariables::hackingKitCount = 0;
uint8_t GameVariables::keyACount = 0;
uint8_t GameVariables::keyBCount = 0;
uint8_t GameVariables::keyCCount = 0;
uint16_t GameVariables::dollarCount = 15;
uint16_t GameVariables::acquiredBlueprints = 0;
uint16_t GameVariables::unlockedBlueprints = 0;

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

uint8_t GameVariables::keysA() { return keyACount; }

uint8_t GameVariables::keysB() { return keyBCount; }

uint8_t GameVariables::keysC() { return keyCCount; }

uint8_t GameVariables::hackingKits() { return hackingKitCount; }

uint16_t GameVariables::dollars() { return dollarCount; }
