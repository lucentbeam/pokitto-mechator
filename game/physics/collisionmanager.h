#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <cstdint>

#include "game/utilities/mapmanager.h"

enum Terrain : uint8_t {
    None = 0,
    Wall = 1,
    WaterShallow = 2,
    WaterDeep = 3,
    Grass = 4,
    Mud = 5,
    DestrucableWood = 6,
    DestructableMetal = 7
};

const uint8_t jungletilesterrain[] =
{
2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
2,3,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,
2,2,2,2,2,2,2,2,2,0,0,0,0,1,0,0,1,0,
5,5,5,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,
5,5,5,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,
5,5,5,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,
1,1,1,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,4,4,4,0,0,0,0,0,0,0,0,0,1,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,0,7,6,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,0,7,6,0,0,0,0,0,0,0,0,0,0,0,0,
};

class CollisionManager
{
public:

    static Terrain getTerrainAt(float x, float y);
};

#endif // COLLISIONMANAGER_H
