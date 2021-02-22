#include "collisionmanager.h"

Terrain CollisionManager::getTerrainAt(float x, float y)
{
    return Terrain(jungletilesterrain[MapManager::getTileAt(x,y)]);
}
