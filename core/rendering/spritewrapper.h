#ifndef SPRITEWRAPPER_H
#define SPRITEWRAPPER_H

#include <cstdint>
#include "game/constants.h"

class SpriteWrapper {
    const uint8_t * frameData;
    uint8_t frame_count = 1;
    uint8_t counter = 0;
    uint8_t currentFrame = 0;
    uint8_t countsPerFrame = 0;
public:
    SpriteWrapper() {}
    SpriteWrapper(const uint8_t* sprites, int framecount, float fps) :
        frameData(sprites),
        frame_count(framecount),
        countsPerFrame(1.0f / (physicsTimestep * fps) + 0.5f)
    {

    }

    void update() {
        if (frame_count < 2) {
            return;
        }
        counter++;
        if (counter >= countsPerFrame) {
            counter %= countsPerFrame;
            currentFrame++;
            currentFrame %= frame_count;
        }
    }
    const uint8_t * data() const {
        if (frameData == nullptr) {
            return nullptr;
        }
        return frameData + (2 + frameData[0] * frameData[1]) * currentFrame;
    }
    const uint16_t countsPerCycle() const {
        return uint16_t(countsPerFrame) * frame_count;
    }
};

#endif // SPRITEWRAPPER_H
