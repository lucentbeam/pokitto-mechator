#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include <unordered_set>

#include "game/utilities/vec.h"

class SpawnPoint
{
    const Vec2f m_pos;

    void (* const m_on_approach)(const Vec2f &);

    static std::unordered_set<const SpawnPoint*> s_active_points;

public:

    SpawnPoint(const Vec2f &pos, void (* const onApproach)(const Vec2f &)) : m_pos(pos), m_on_approach(onApproach) {}

    static void setActiveRegion();
};

const SpawnPoint world_points[] = { SpawnPoint({0,0},nullptr) };

#endif // SPAWNPOINT_H
