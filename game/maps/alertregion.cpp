#include "alertregion.h"

#include "game/utilities/mapmanager.h"

RegionName groundregion_names[] = {
    RegionName("Tutorial Island"),
    RegionName("Stormy Cape")
};

RegionBounds groundregion_areas[] = {
    RegionBounds(4, 0, 44, 22, groundregion_names),
    RegionBounds(43, 0, 76, 30, groundregion_names),
    RegionBounds(34, 164, 178, 214, groundregion_names + 1)
};

bool checkGroundRegions(const char *&n) {
    Vec2f cp = Camera::center();
    if (MapManager::getTileAt(cp.x(), cp.y()) < 54) return false;
    bool triggered = false;
    for(RegionBounds &r : groundregion_areas) {
        if (!r.occupied && r.contains(cp)) {
            r.occupied = true;
            ++r.reference_region->count;
            if (r.reference_region->count == 1) {
                triggered = true;
                n = r.reference_region->name;
            }
        }
    }
    for(RegionBounds &r : groundregion_areas) {
        if (r.occupied && !r.contains(cp)) {
            r.occupied = false;
            --r.reference_region->count;
        }
    }
    return triggered;
}
