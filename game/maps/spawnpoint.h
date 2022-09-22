#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "core/utilities/vec.h"
#include "game/enums.h"

class SpawnPoint
{

public:
    enum SpawnPointType : uint8_t {
        Shop = 0,

        DoorA = 1,
        DoorB = 2,
        DoorC = 3,
        DoorNone = 4,

        KeyA,
        KeyB,
        KeyC,

        HackingKit,
        Blueprint,

        EnemyBarracks,
        Mine,
        WaterMine,
        Boat,
        Tank,
        Mech,
        Turret,
        TurretDisabled,
        Bomber,
        Helicopter,
        LasersVert,
        LasersHoriz
    };

    static const POIType door_labels[];

    constexpr SpawnPoint(const Vec2i &pos, const SpawnPointType t): m_pos(pos * 6), m_type(t) {}

    static void setActiveRegion();

    static void reset();

    Vec2f pos() const { return {float(m_pos.x()), float(m_pos.y())}; }

    static void openDoorAt(const Vec2f &pos);

    static void approach(const SpawnPoint &s, int idx);

    SpawnPointType type() const { return m_type; }

private:

    const Vec2i m_pos;

    const SpawnPointType m_type;

    static bool s_active_points[];
};

#endif // SPAWNPOINT_H
