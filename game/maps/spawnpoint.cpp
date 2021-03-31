#include "spawnpoint.h"

#include "core/rendering/camera.h"
#include <algorithm>

#include "game/maps/spawnpoints.h"

std::unordered_set<const SpawnPoint*> SpawnPoint::s_active_points;

void SpawnPoint::setActiveRegion()
{
    if (!Camera::hasMovedRegions()) return;

    std::unordered_set<const SpawnPoint*> next;

    for (int i = 0; i < point_count; ++i) {
        if (Camera::inActiveZone(points[i].m_pos)) {
            if (s_active_points.find(points + i) == s_active_points.end()) {
                points[i].m_on_approach(points[i].m_pos);
            }
            next.insert(points + i);
        }
    }
    s_active_points = next;
}
