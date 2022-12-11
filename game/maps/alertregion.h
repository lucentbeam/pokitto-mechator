#ifndef ALERTREGIONS_H
#define ALERTREGIONS_H

#include "core/utilities/rect.h"
#include "core/rendering/camera.h"
#include "game/enums.h"

struct RegionName {
    const char * name;
    RegionNames region_enum;
    bool occupied = false;

    // enum Mode { ThrowOnEnter, ThrowOnExit } mode;  // consider this for future use
    RegionName(const char * n, RegionNames rn) : name(n), region_enum(rn) {}
};

struct RegionBounds : public Rect
{
    RegionName * const reference_region;

    constexpr RegionBounds(int x, int y, int x2, int y2, RegionName * const ref) : Rect(x * 6, y * 6, (x2 - x + 1) * 6, (y2 - y + 1) * 6), reference_region(ref) {}
};

extern RegionName groundregion_names[];

extern RegionBounds groundregion_areas[];

bool checkGroundRegions(const char * &n, bool boatmode);

bool isInRegion(RegionNames rn);

RegionNames currentRegion();

#endif // ALERTREGIONS_H
