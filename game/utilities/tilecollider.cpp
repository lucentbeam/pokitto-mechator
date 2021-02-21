#include "tilecollider.h"

std::set<uint8_t> TileCollider::tilesHitting() const
{
    return std::set<uint8_t>();
}

bool TileCollider::isColliding() const
{
    std::set<uint8_t> corners = tilesHitting();
    for(uint8_t idx : m_collisions) {
        if (corners.find(idx) != corners.end()) {
            return true;
        }
    }
    return false;
}
