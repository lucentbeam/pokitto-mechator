#ifndef VARIABLES_H
#define VARIABLES_H

#include <cstdint>
#include <cmath>

#include "core/utilities/vec.h"
#include "game/enums.h"
#include "game/constants.h"

struct GameStorageHeader {
    int elapsedMilliseconds = 0;
    bool visitedEvents[SequenceID::LastID] = { false };
    float percentageComplete() { return 50.0f; }
};

struct GameStorage {
    GameStorageHeader header;

    uint8_t hackingKitCount = 0, keyACount = 0, keyBCount = 0, keyCCount = 0;
    uint16_t dollarCount = 50;

    uint16_t acquiredBlueprints = 0;
    uint16_t unlockedBlueprints = 0;

    uint8_t questStatus = 0;

    DoorStatus door_states[doors_count];
    int activated_doors[points_count];
    int acquired_specials[specials_count];
};

class GameVariables {
    static bool gameWon;

    static GameStorage s_data;

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

    static char savefile[];

    static void updateTime(int ms);
    static void loadGame(GameStorage);
    static void saveGame();

    static DoorStatus * doorStates();
    static int * activatedDoors();
    static int * acquiredSpecials();
};



#endif // VARIABLES_H
