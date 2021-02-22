#include "collisionmanager.h"

bool CollisionManager::collisionOn(const Rect &rect, const std::set<uint8_t> collisions)
{
    float x = rect.left();
    float y = rect.top();
    if (collisions.find(getTerrainAt(x,y)) != collisions.end()) {
        return true;
    }
    x = rect.right();
    if (collisions.find(getTerrainAt(x,y)) != collisions.end()) {
        return true;
    }
    y = rect.bottom();
    if (collisions.find(getTerrainAt(x,y)) != collisions.end()) {
        return true;
    }
    x = rect.left();
    if (collisions.find(getTerrainAt(x,y)) != collisions.end()) {
        return true;
    }
    return false;
}

Terrain CollisionManager::getTerrainAt(float x, float y)
{
    return Terrain(jungletilesterrain[MapManager::getTileAt(x,y)]);
}

Vec2f CollisionManager::resolveMovement(const Vec2f &pos, const Vec2f &delta, const std::set<uint8_t> &collisions, const Vec2f &size)
{
    Vec2f next = pos + delta;
    Rect rect = Rect(next.x() - size.x()/2, next.y() - size.y()/2, size.x(), size.y());
    if (!collisionOn(rect, collisions)) {
        return next;
    }
    Vec2f dp = Vec2f(delta.x(), 0.0f);
    next = pos + dp;
    rect.setCenter(next.x(), next.y());
    if (!collisionOn(rect, collisions)) {
        return next;
    }
    dp = Vec2f(0.0f, delta.y());
    next = pos + dp;
    rect.setCenter(next.x(), next.y());
    if (!collisionOn(rect, collisions)) {
        return next;
    }
    return pos;
}

