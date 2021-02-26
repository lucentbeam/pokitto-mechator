#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <cstdint>

#include "game/utilities/mapmanager.h"
#include "core/utilities/rect.h"

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

const uint8_t jungletilesterrain[] =
{
2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
2,3,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,
2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,
5,5,5,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,
5,5,5,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
5,5,5,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,0,4,4,4,1,0,0,0,0,0,0,0,0,0,0,
1,1,1,0,4,4,4,1,1,0,0,0,0,0,0,0,0,0,
1,1,1,1,4,4,4,1,1,0,0,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,7,6,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,7,6,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

class CollisionManager
{
    static bool rectCollides(const Rect &rect, uint16_t collisionMask);

public:
    static bool collision(const Vec2f &pos, uint16_t collisionMask, const Vec2f &size);

    static Terrain getTerrainAt(float x, float y);

    static Vec2f resolveMovement(Vec2f pos, const Vec2f &delta, uint16_t collisionMask, const Vec2f &size);
};

#endif // COLLISIONMANAGER_H
