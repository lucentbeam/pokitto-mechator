#include "collisionmanager.h"

bool CollisionManager::rectCollides(const Rect &rect, uint16_t collisionMask)
{
    int xlower = rect.left()/6.0f;
    int xupper = rect.right()/6.0f;
    int ylower = rect.top()/6.0f;
    int yupper = rect.bottom()/6.0f;

    for(int px = xlower; px <= xupper; px++) {
        for (int py = ylower; py <= yupper; py++) {
            if ((1 << getTerrainAt(px*6,py*6) & collisionMask) > 0) {
                return true;
            }
        }
    }
    return false;
}

bool CollisionManager::collision(const Vec2f &pos, uint16_t collisionMask, const Vec2f &size)
{
    Rect rect = Rect(0, 0, size.x(), size.y());
    rect.setCenter(pos.x(), pos.y());
    return rectCollides(rect, collisionMask);
}

bool CollisionManager::collides(const Vec2f &pos, uint16_t collisionMask)
{
    return (1 << (getTerrainAt(pos.x(), pos.y())) & collisionMask) > 0;
}

Terrain CollisionManager::getTerrainAt(float x, float y)
{
    return Terrain(jungletilesterrain[MapManager::getTileAt(x,y)]);
}

Vec2f CollisionManager::resolveMovement(Vec2f pos, const Vec2f &delta, uint16_t collisionMask, const Vec2f &size)
{
    Vec2f next = pos + delta;
    Rect rect = Rect(0, 0, size.x(), size.y());

    rect.setCenter(next.x(), next.y());
    if (!rectCollides(rect, collisionMask)) {
        return next;
    }

    Vec2f dp = Vec2f(0.0f, delta.y());
    next = pos + dp;
    rect.setCenter(next.x(), next.y());
    if (!rectCollides(rect, collisionMask)) {
        return next;
    }

    dp = Vec2f(delta.x(), 0.0f);
    next = pos + dp;
    rect.setCenter(next.x(), next.y());
    if (!rectCollides(rect, collisionMask)) {
        return next;
    }

    return pos;
}

Vec2f CollisionManager::resolveMovement(const Vec2f &pos, const Vec2f &delta, uint16_t collisionMask)
{
    Vec2f next = pos + delta;

    if ((1 << getTerrainAt(next.x(),next.y()) & collisionMask) == 0) {
        return next;
    }

    Vec2f dp = Vec2f(0.0f, delta.y());
    next = pos + dp;
    if ((1 << getTerrainAt(next.x(),next.y()) & collisionMask) == 0) {
        return next;
    }

    dp = Vec2f(delta.x(), 0.0f);
    next = pos + dp;
    if ((1 << getTerrainAt(next.x(),next.y()) & collisionMask) == 0) {
        return next;
    }

    return pos;
}

