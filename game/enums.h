#ifndef ENUMS_H
#define ENUMS_H

#include <cstdint>

enum DoorStatus : uint8_t {
    Hidden = 0,
    Discovered = 1,
    Opened = 2
};

enum SequenceID : int {
    Intro = 0,
    TutorialBoss = 1,
    TankBoss = 2,
    AcquireBoat = 3,
    EnterHelicopter = 4,
    FinalBoss = 5,
    ExitTutorial = 6,
    EnterBoat = 7,
    EnterTank = 8,
    EnterJeep = 9,
    GetBluePrint = 10,

    LastID = 11
};

enum PlayerMode : uint8_t {
    SoldierMode = 0,
    JeepMode = 1,
    TankMode = 2,
    BoatMode = 3,
    HelicopterMode = 4
};

enum Terrain : uint16_t {
    None = 0,
    Wall = 1,
    WaterShallow = 2,
    WaterDeep = 3,
    Grass = 4,
    Mud = 5,
    DestrucableWood = 6,
    DestructableMetal = 7,
    LowWall = 8,
};

enum POIType : uint8_t {
    Shop = 0,
    DoorA = 1,
    DoorB = 2,
    DoorC = 3,
    DoorNone = 4
};

enum SpecialTiles : uint8_t {
    UnexplodedOrdinance = 63,
    ExplodedOrdinance = 64,
    BaseGround = 184,
    DestroyedTurret = 185
};

enum GameStates : uint8_t {
    Game,
    ShowUnlockDoor,
    ShowUnlockShop,
    ShowShop,
    Pause,
    ShowRepairs,
    ShowBlueprint,
    ShowBlueprintShop,
    EventState,
    MapState,
    GameWonState,
    TitleState,
    GameOverState
};

enum QuestStatus : uint8_t {
    QuestIntro = 0,
    QuestTank = 1,
    QuestBoat = 2,
    QuestBuildBoat = 3,
    QuestHeli = 4,
    QuestFinal = 5
};

enum RegionNames : uint8_t {
    RegionNone,
    RegionTutorial,
    RegionStormyCape,
    RegionTankFactory,
    RegionBoatYard,
    RegionEnemyFactory,
};

enum Blueprints : uint8_t {
    JeepBP              = 0,
    TankBP              = 1,
    BoatBP              = 2,
    HeliBP              = 3,
    SpreaderShotBP      = 4,  // jeep, boat
    TacticalArmorBP     = 5,  // soldier, player.cpp
    NinjaShoesBP        = 6,  // soldier, player.cpp
    FlamethrowerBP      = 7,  // tank
    MultinadeBP         = 8,  // jeep
    ClustershotBP       = 9,  // heli

    AllTerrainTiresBP   = 10, // jeep, player.cpp
    UnobtainiumHullBP   = 11, // heli, player.cpp

    MachineGunBP        = 12, // tank, boat, soldier

    CounterMotorBP      = 13, // boat

    LastIdxBP           = 20
};

#endif // ENUMS_H
