#include "spawnpoint.h"

#include "core/rendering/camera.h"
#include <algorithm>

#include "game/maps/spawnpoints.h"
#include "game/maps/doors.h"

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

SpawnPoint::DoorStatus SpawnPoint::door_states[door_count] = {
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
    SpawnPoint::Hidden,
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
            if (door_states[i] == SpawnPoint::Hidden) {
                door_states[i] = SpawnPoint::Discovered;
            }
        } else {
            s_active_doors[i] = false;
        }
    }
}

void SpawnPoint::openDoorAt(const Vec2f &pos)
{
    for(int i = 0; i < door_count; ++i) {
        if (doors[i].pos().x() == int(pos.x()) && doors[i].pos().y() == int(pos.y())) {
            door_states[i] = SpawnPoint::Opened;
        }
    }
}

