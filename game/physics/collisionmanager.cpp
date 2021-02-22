#include "collisionmanager.h"

bool CollisionManager::rectCollides(const Rect &rect, uint16_t collisionMask)
{
    float x = rect.left();
    float y = rect.top();
    int xdivisions = int(rect.width() / 6) + 1;
    float xstep = rect.width() / float(xdivisions);
    int ydivisions = int(rect.height() / 6) + 1;
    float ystep = rect.height() / float(ydivisions);
    for(int i = 0; i <= xdivisions; i++) {
        for (int j = 0; j <= ydivisions; j++) {
            float px = x + float(i) * xstep;
            float py = y + float(j) * ystep;
            if ((1 << getTerrainAt(px,py) & collisionMask) > 0) {
                return true;
            }
        }
    }
    return false;
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

