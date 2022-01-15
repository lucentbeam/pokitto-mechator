#ifndef ENUMS_H
#define ENUMS_H

#include <cstdint>

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
    ShowBlueprint
};

enum Blueprints {
    JeepBP              = 0,
    TankBP              = 1,
    BoatBP              = 2,
    HeliBP              = 3,
    GeminiGunBP         = 4,
    TacticalArmorBP     = 5,
    NinjaShoesBP        = 6,
    SpreaderShotBP      = 7,
    MultinadeBP         = 8,
    ClustershotBP       = 9,

    LastIdxBP           = 10
};

#endif // ENUMS_H
