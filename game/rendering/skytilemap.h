#ifndef SKYTILEMAP_H
#define SKYTILEMAP_H

#include <cstdint>
#include <map>
#include <vector>

#include "core/rendering/rendersystem.h"
#include "game/rendering/camera.h"
#include "core/rendering/screenbuffer.h"

class SkyTilemap {
  const uint8_t (*m_tiles)[18*18+2];

  const uint8_t * m_map;
  const uint8_t m_mapwidth, m_mapheight;

public:
  const uint8_t render_width = 7, render_height = 6;

  SkyTilemap(const uint8_t tiles[][18*18+2], const uint8_t * map);

  void draw();

};

#endif // SKYTILEMAP_H
