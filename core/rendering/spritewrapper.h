#ifndef SPRITEWRAPPER_H
#define SPRITEWRAPPER_H

#include <vector>
#include <cstdint>

class SpriteWrapper {
    std::vector<const uint8_t*> frameData;
    uint8_t counter = 0;
    uint8_t currentFrame = 0;
    uint8_t countsPerFrame = 0;
public:
    SpriteWrapper() {}
    SpriteWrapper(std::initializer_list<const uint8_t*> sprites, float fps) :
        frameData(sprites),
        countsPerFrame(1.0f / (0.014f * fps) + 0.5f)
    {

    }

    void update() {
        if (frameData.size() < 2) {
            return;
        }
        counter++;
        if (counter >= countsPerFrame) {
            counter %= countsPerFrame;
            currentFrame++;
            currentFrame %= frameData.size();
        }
    }
    const uint8_t * data() const {
        if (frameData.size() < 1) {
            return nullptr;
        }
        return frameData[currentFrame];
    }
    const uint16_t countsPerCycle() const {
        return uint16_t(countsPerFrame) * frameData.size();
    }
};

#endif // SPRITEWRAPPER_H
