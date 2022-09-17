#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "core/utilities/vec.h"
#include "game/enums.h"

class SpawnPoint
{
    const Vec2i m_pos;

    static bool s_active_points[];
    static bool s_active_doors[];

public:
    static const POIType door_labels[];

    constexpr SpawnPoint(const Vec2i &pos, void (* const onApproach)(const Vec2i &)): m_pos(pos * 6), m_on_approach(onApproach) {}

    static void setActiveRegion();

    static void reset();

    Vec2f pos() const { return {float(m_pos.x()), float(m_pos.y())}; }

    static void openDoorAt(const Vec2f &pos);

    void (* const m_on_approach)(const Vec2i &);
};

#endif // SPAWNPOINT_H
