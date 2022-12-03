#include "spritewrapper.h"

uint16_t SpriteWrapper::s_counter = 0;

void SpriteWrapper::update() { s_counter++; }

int SpriteWrapper::currentFrame() const {
    int ct = s_counter - counter;
    if (s_counter < counter) {
        ct = int(65535 - counter) + int(s_counter);
    }
    ct /= sprite_configs[int(sprite)].counts_per_frame;
    int fc = sprite_configs[int(sprite)].frame_count;
    return ct % fc;
}

const uint8_t *SpriteWrapper::data() const {
    if (sprite == NoSprite) {
        return nullptr;
    }
    const SpriteConfig config = sprite_configs[int(sprite)];
    return config.sprite + (2 + config.sprite[0] * config.sprite[1]) * currentFrame();
}

uint16_t SpriteWrapper::countsPerCycle() const {
    const SpriteConfig config = sprite_configs[int(sprite)];
    return uint16_t(config.counts_per_frame) * config.frame_count;
}

