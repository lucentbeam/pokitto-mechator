#ifndef VARIABLES_H
#define VARIABLES_H

#include <cstdint>
#include <cmath>

#include "core/utilities/vec.h"

class GameVariables {
    static uint8_t hackingKitCount, keyACount, keyBCount, keyCCount;
    static uint16_t dollarCount;

    static uint16_t acquiredBlueprints;
    static uint16_t unlockedBlueprints;

    static uint8_t questStatus;

    static bool gameWon;

    static bool visitedEvents[];
public:
    static void changeDollars(int16_t delta);
    static void changeHackingKits(int8_t delta);
    static void changeKeysA(int8_t delta);
    static void changeKeysB(int8_t delta);
    static void changeKeysC(int8_t delta);

    static bool hasBlueprint(int bp);
    static void acquireBlueprint(int bp);

    static bool hasBlueprintUnlocked(int bp);
    static void unlockBlueprint(int bp);

    static bool hasBlueprintToUnlock(int bp);

    static bool hasUnusedBlueprints();

    static uint8_t keysA();
    static uint8_t keysB();
    static uint8_t keysC();
    static uint8_t hackingKits();
    static uint16_t dollars();

    static void visitEvent(int index);
    static bool eventVisited(int index);

    static void setQuestStatus(uint8_t q);
    static Vec2i getGoal();

    static void setGameWon();
    static bool getGameWon();
};

#endif // VARIABLES_H
