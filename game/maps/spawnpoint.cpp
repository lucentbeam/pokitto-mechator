#include "spawnpoint.h"

#include "game/rendering/camera.h"
#include <algorithm>

std::unordered_set<const SpawnPoint*> SpawnPoint::s_active_points;

void SpawnPoint::setActiveRegion(const SpawnPoint points[], uint16_t point_count)
{
    if (!Camera::hasMovedRegions()) return;

    std::unordered_set<const SpawnPoint*> prev;
    prev.insert(s_active_points.begin(), s_active_points.end());
    s_active_points.clear();

    for (int i = 0; i < point_count; i++) {
        const SpawnPoint pt = points[i];
        if (Camera::inActiveZone(pt.m_pos)) {
            s_active_points.insert(&pt);
            if (prev.find(&pt) == prev.end()) {
                pt.m_on_approach(pt.m_pos);
            }
        }
    }
}
