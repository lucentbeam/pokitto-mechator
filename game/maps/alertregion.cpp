#include "alertregion.h"

#include "game/utilities/mapmanager.h"
#include <string.h>

RegionName groundregion_names[] = {
    RegionName("Tutorial Island", RegionTutorial),
    RegionName("Stormy Cape", RegionStormyCape),
    RegionName("Tank Factory", RegionTankFactory),
    RegionName("Destroyed Boat Yard", RegionBoatYard),
    RegionName("Central Factory", RegionEnemyFactory)
};

RegionBounds groundregion_areas[] = {
    // tutorial
    RegionBounds(4, 0, 44, 22, groundregion_names),
    RegionBounds(43, 0, 76, 30, groundregion_names),

    // cape
    RegionBounds(22, 161, 102, 189, groundregion_names + 1),
    RegionBounds(97, 178, 147, 208, groundregion_names + 1),

    // tank factory
    RegionBounds(9, 85, 43, 113, groundregion_names + 2),

    // boatyard
    RegionBounds(147, 191, 176, 213, groundregion_names + 3),

    // finalboss
    RegionBounds(91, 134, 125, 152, groundregion_names + 4),
    RegionBounds(106, 150, 126, 168, groundregion_names + 4),
    RegionBounds(123, 156, 142, 178, groundregion_names + 4),
};

bool checkGroundRegions(const char *&n) {
    Vec2f cp = Camera::center();
//    if (MapManager::getTileAt(cp.x(), cp.y()) < 54) return false;
    RegionNames previous = currentRegion();
    for(RegionName &r : groundregion_names) {
        r.occupied = false;
    }
    for(RegionBounds &r : groundregion_areas) {
        if (r.contains(cp)) {
            r.reference_region->occupied = true;
            n = r.reference_region->name;
            break;
        }
    }
    RegionNames next = currentRegion();
    if (next == RegionNone) {
        n = "";
    }
    return next != previous;
}

bool isInRegion(RegionNames rn)
{
//    Vec2f cp = Camera::center();
//    if (MapManager::getTileAt(cp.x(), cp.y()) < 54) return false;
    for(RegionBounds &r : groundregion_areas) {
        if (r.reference_region->occupied) {
            return r.reference_region->region_enum == rn;
        }
    }
    return rn == RegionNone;
}

RegionNames currentRegion()
{
    for(RegionBounds &r : groundregion_areas) {
        if (r.reference_region->occupied) {
            return r.reference_region->region_enum;
        }
    }
    return RegionNone;
}
