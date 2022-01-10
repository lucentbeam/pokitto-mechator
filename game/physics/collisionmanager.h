#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <cstdint>

#include "game/utilities/mapmanager.h"
#include "core/utilities/rect.h"
#include "game/enums.h"

const uint8_t jungletilesterrain[] =
{
    3,3,3,2,2,2,2,3,3,3,3,3,3,3,3,0,0,0,
    3,3,3,2,2,2,2,3,3,3,3,3,3,1,1,3,0,0,
    3,3,3,2,2,2,2,3,3,3,3,3,3,1,1,1,1,1,
    5,5,5,1,1,1,1,0,1,0,0,1,0,4,4,4,0,0,
    5,5,5,1,1,1,1,0,0,0,0,1,1,4,4,4,0,0,
    5,5,5,1,1,1,1,0,0,0,0,1,1,4,4,4,0,0,
    1,1,1,0,1,1,1,0,1,1,1,0,8,8,8,0,0,0,
    1,1,1,0,1,1,1,0,1,1,1,0,8,0,8,8,8,0,
    1,1,1,1,1,1,1,1,1,8,8,8,8,8,8,8,8,0,
    1,1,1,0,0,0,1,1,1,8,8,8,0,0,0,0,0,0,
    1,1,1,0,0,0,1,1,1,8,0,8,0,0,0,1,1,1,
    1,1,1,0,0,0,1,1,1,8,8,8,0,0,0,1,1,1,
    1,1,1,7,6,0,0,0,0,8,8,8,0,0,0,1,1,1,
    1,1,1,7,6,1,1,0,0,8,8,8,0,0,0,0,0,0,
};

class CollisionManager
{
    static bool rectCollides(const Rect &rect, uint16_t collisionMask);

public:
    static bool collision(const Vec2f &pos, uint16_t collisionMask, const Vec2f &size);

    static bool collides(const Vec2f &pos, uint16_t collisionMask);

    static Terrain getTerrainAt(float x, float y);

    static Vec2f resolveMovement(Vec2f pos, const Vec2f &delta, uint16_t collisionMask, const Vec2f &size);

    static Vec2f resolveMovement(const Vec2f &pos, const Vec2f &delta, uint16_t collisionMask);
};

#endif // COLLISIONMANAGER_H
