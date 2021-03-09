#ifndef ENUMS_H
#define ENUMS_H

#include <cstdint>

enum PlayerMode : uint8_t {
    Soldier = 1,
    Jeep = 2,
    Tank = 3,
    Boat = 4,
    Helicopter = 5
};

enum Terrain : uint16_t {
    None = 0,
    Wall = 1,
    WaterShallow = 2,
    WaterDeep = 3,
    Grass = 4,
    Mud = 5,
    DestrucableWood = 6,
    DestructableMetal = 7
};

enum POIType : uint8_t {
    Shop = 0,
    DoorA = 1,
    DoorB = 2,
    DoorC = 3
};

enum SpecialTiles : uint8_t {
    BaseGround = 184
};

enum GameStates : uint8_t {
    Game,
    ShowUnlockDoor,
    ShowUnlockShop,
    ShowShop,
    Pause
};

#endif // ENUMS_H
