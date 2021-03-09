#include "spawnpoint.h"

#include "game/rendering/camera.h"
#include <algorithm>

#include "game/entities/enemymech.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"

std::unordered_set<const SpawnPoint*> SpawnPoint::s_active_points;

const SpawnPoint points[] = {
    SpawnPoint({18*6, 15*6}, Pickups::spawnKeycardA),
    SpawnPoint({15*6, 7*6}, Enemy::createMech),
    SpawnPoint({38*6, 7*6}, Enemy::createMech),
    SpawnPoint({43*6, 10*6}, spawnBarracks<42,7,4,3>),
    SpawnPoint({60*6, 13*6}, spawnBarracks<59,10,3,3>),
    SpawnPoint({68*6, 13*6}, spawnBarracks<67,10,3,3>),
    SpawnPoint({68*6, 24*6}, spawnBarracks<67,21,3,3>),
    SpawnPoint({60*6, 24*6}, spawnBarracks<59,21,3,3>),
    SpawnPoint({45*6, 16*6}, spawnDoorA<44,15,1,3>),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);

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
