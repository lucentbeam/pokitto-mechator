#include "spawnpoint.h"

#include "core/rendering/camera.h"
#include <algorithm>

#include "game/maps/spawnpoints.h"
#include "game/maps/doors.h"
#include "game/variables.h"

bool SpawnPoint::s_active_points[point_count];
bool SpawnPoint::s_active_doors[door_count] = { false };

const POIType SpawnPoint::door_labels[door_count] = {
    POIType::DoorA,
    POIType::DoorA,
    POIType::DoorA,
    POIType::DoorA,

    POIType::DoorB,
    POIType::DoorB,

    POIType::DoorC,
    POIType::DoorC,
    POIType::DoorC,
    POIType::DoorC,

    POIType::DoorB,
    POIType::DoorA,
};

void SpawnPoint::setActiveRegion()
{
    if (!Camera::hasMovedRegions()) return;

    for (int i = 0; i < point_count; ++i) {
        if (Camera::inActiveZone(points[i].m_pos)) {
            if (!s_active_points[i]) {
                points[i].m_on_approach(points[i].m_pos);
            }
            s_active_points[i] = true;
        } else {
            s_active_points[i] = false;
        }
    }

    for (int i = 0; i < door_count; ++i) {
        if (Camera::inActiveZone(doors[i].m_pos)) {
            if (!s_active_doors[i]) {
                doors[i].m_on_approach(doors[i].m_pos);
            }
            s_active_doors[i] = true;
            if (GameVariables::doorStates()[i] == Hidden && Camera::inViewingZone(doors[i].m_pos)) {
                GameVariables::doorStates()[i] = Discovered;
            }
        } else {
            s_active_doors[i] = false;
        }
    }
}

void SpawnPoint::reset()
{
    for (int i = 0; i < point_count; ++i) s_active_points[i] = false;
    for (int i = 0; i < door_count; ++i) s_active_doors[i] = false;
}

void SpawnPoint::openDoorAt(const Vec2f &pos)
{
    for(int i = 0; i < door_count; ++i) {
        if (doors[i].pos().x() == int(pos.x()) && doors[i].pos().y() == int(pos.y())) {
            GameVariables::doorStates()[i] = Opened;
        }
    }
}

