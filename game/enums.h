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

#endif // ENUMS_H
