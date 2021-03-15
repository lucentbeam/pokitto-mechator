#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

const float uiEasingTime = 0.2f;
const uint8_t uiEasingTimeMs = uint8_t(uiEasingTime * 1000);

const float physicsTimestep = 0.01f;
const uint8_t physicsTimestepMs = uint8_t(physicsTimestep * 1000);

#endif // CONSTANTS_H
