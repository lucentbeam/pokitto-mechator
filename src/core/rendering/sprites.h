#ifndef _SPRITES
#define _SPRITES

#include <stdint.h>

struct SpriteData {
  SpriteData(const uint8_t * pix = nullptr);
  const uint8_t * pixels;
};

#endif
