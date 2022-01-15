#ifndef VARIABLES_H
#define VARIABLES_H

#include <cstdint>
#include <cmath>

class GameVariables {
    static uint8_t hackingKitCount, keyACount, keyBCount, keyCCount;
    static uint16_t dollarCount;

    static uint16_t acquiredBlueprints;
    static uint16_t unlockedBlueprints;
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

    static uint8_t keysA();
    static uint8_t keysB();
    static uint8_t keysC();
    static uint8_t hackingKits();
    static uint16_t dollars();
};

#endif // VARIABLES_H
