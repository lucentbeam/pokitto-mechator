#ifndef TILECOLLIDER_H
#define TILECOLLIDER_H

#include <cstdint>
#include <initializer_list>
#include <set>

#include "core/utilities/rect.h"

class TileCollider : public Rect
{
    const std::initializer_list<uint8_t> m_collisions;

    std::set<uint8_t> tilesHitting() const;

public:
    TileCollider(const std::initializer_list<uint8_t> collisions) : m_collisions(collisions) {}

    bool isColliding() const;
};

#endif // TILECOLLIDER_H
