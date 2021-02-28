#include "regionmanager.h"

#include <algorithm>
#include <cmath>

const int tileSize = 6;
const int mapCount = mapSizeTiles / regionSizeTiles;
const int regionSize = regionSizeTiles * tileSize;

std::vector<RegionRect*> RegionRect::s_regions[(mapSizeTiles/regionSizeTiles)*(mapSizeTiles/regionSizeTiles)];

void RegionRect::validate()
{
    int l = std::floor(left() / regionSize);
    int r = std::floor(right() / regionSize);
    int t = std::floor(top() / regionSize);
    int b = std::floor(bottom() / regionSize);

    std::unordered_set<int> indices;
    indices.insert(l + t * mapCount);
    indices.insert(r + t * mapCount);
    indices.insert(l + b * mapCount);
    indices.insert(r + t * mapCount);

    for (int idx : m_regions_occupied) {
        if (indices.find(idx) == indices.end()) {
            auto it = std::remove_if(s_regions[idx].begin(), s_regions[idx].end(), [this](RegionRect*r) { return r == this; });
            s_regions[idx].erase(it);
        }
    }
    for (int idx : indices) {
        if (m_regions_occupied.find(idx) == m_regions_occupied.end()) {
            s_regions[idx].push_back(this);
        }
    }
    m_regions_occupied = indices;
}

void RegionRect::move(float new_x, float new_y)
{
    setCenter(new_x, new_y);
    validate();
}

std::unordered_set<RegionRect *> RegionRect::getInRange(float x, float y, float w, float h)
{
    int l = std::floor(x / regionSize);
    int r = std::floor((x+w) / regionSize);
    int t = std::floor(y / regionSize);
    int b = std::floor((y+h) / regionSize);

    std::unordered_set<RegionRect*> output;
    for(int i = l; i <= r; i++) {
        if (i < 0 || i >= mapCount) continue;
        for (int j = t; j <= b; j++) {
            if (j < 0 || j >= mapCount) continue;
            int idx = i + j * mapCount;
            if (s_regions[idx].size() > 0) {
                output.insert(s_regions[idx].begin(), s_regions[idx].end());
            }
        }
    }
    return output;
}
