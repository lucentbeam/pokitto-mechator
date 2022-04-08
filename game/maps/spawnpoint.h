#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include <unordered_set>

#include "core/utilities/vec.h"

class SpawnPoint
{
    const Vec2i m_pos;


    static bool s_active_points[];

public:

    constexpr SpawnPoint(const Vec2i &pos, void (* const onApproach)(const Vec2i &)): m_pos(pos * 6), m_on_approach(onApproach) {}

    static void setActiveRegion();


    void (* const m_on_approach)(const Vec2i &);
};

#endif // SPAWNPOINT_H
