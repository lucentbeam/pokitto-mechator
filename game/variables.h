#ifndef VARIABLES_H
#define VARIABLES_H

#include <cstdint>
#include <cmath>

#include "core/utilities/vec.h"
#include "game/enums.h"
#include "game/constants.h"
#include "game/maps/worldtiles.h"

#ifdef DEBUGS
struct DebugOptions {
    static bool noclip;
};
#endif

struct GameStorageHeader {
    int elapsedMilliseconds = 0;
    bool visitedEvents[SequenceID::LastID] = { false };
    float percentageComplete() { return 50.0f; }
};

struct GameOptions {
    bool mus_on = true;
    bool sfx_on = true;

    static GameOptions s_options;

public:
    static void initialize();
    static void save();

    static bool musicOn();
    static void setMusicOn(bool value);

    static bool sfxOn();
    static void setSfxOn(bool value);

    static float volumeFrac();
    static void setVolumeFrac(float value);
};

struct GameStorage {
    GameStorageHeader header;

    uint8_t hackingKitCount = 0, keyACount = 0, keyBCount = 0, keyCCount = 0;
    uint16_t dollarCount = 30;

    uint32_t acquiredBlueprints = 0;
    uint32_t unlockedBlueprints = 0;

    uint8_t questStatus = 0;

    DoorStatus door_states[spawnpoint_data_count] = { DoorStatus::Hidden };
    int activated_doors[doors_count] = {0};
    int acquired_specials[specials_count] = {0};

    Vec2f soldierPosition, jeepPosition, tankPosition, boatPosition, heliPosition;
    int8_t soldierLife, jeepLife, tankLife, boatLife, heliLife;
};

class GameVariables {
    static bool gameWon;

    static GameStorage s_data;

public:
    static void changeDollars(int16_t delta);
    static void changeDollarsBig(int8_t delta);
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

    static GameStorage * getData();
};



#endif // VARIABLES_H
