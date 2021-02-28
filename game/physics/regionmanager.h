#ifndef REGIONMANAGER_H
#define REGIONMANAGER_H

#include <vector>
#include <unordered_set>

#include "core/utilities/rect.h"
#include "core/rendering/rendersystem.h"

const int mapSizeTiles = 256;
const int regionSizeTiles = 16;

class RegionRect : public Rect
{
    static std::vector<RegionRect*> s_regions[(mapSizeTiles/regionSizeTiles)*(mapSizeTiles/regionSizeTiles)];

    std::unordered_set<int> m_regions_occupied;
public:
    RegionRect(float x, float y, float w, float h) : Rect(x-w/2, y-w/2, w, h) { validate(); }

    void validate();

    void move(float new_x, float new_y);

    static std::vector<RegionRect*> overlapsWith(const Rect &rect);

    static std::unordered_set<RegionRect*> getInRange(float x, float y, float w, float h);

    void * parent;
};

#endif // REGIONMANAGER_H
