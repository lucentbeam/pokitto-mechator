#include "spawnpoint.h"

#include "core/rendering/camera.h"
#include <algorithm>

#include "game/maps/spawnpoints.h"

bool SpawnPoint::s_active_points[point_count];

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
}

