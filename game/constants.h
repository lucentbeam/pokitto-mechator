#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

const float uiEasingTime = 0.2f;
const uint8_t uiEasingTimeMs = uint8_t(uiEasingTime * 1000);

constexpr float physicsTimestep = 0.012f;
const uint8_t physicsTimestepMs = uint8_t(physicsTimestep * 1000);

const int barracksMaxMoneyDrops = 5;

const float enemyHeliSpeed = 20.0f;
const float enemyTankSpeed = 12.0f;

// for final: 8, 15
const int playerStartTileX = 8;//18;
const int playerStartTileY = 15;//33;

const int playerIframeLength = 8;

const float soldierSpeed = 20.0f;

const float jeepSpeed = 50.0f;
const float jeepGrassSpeedFraction = 0.64f;
const float jeepCornering = 0.1f;
const float jeepFriction = 0.05f;

const float tankSpeed = 32.0f;
const float tankCornering = 0.04f;

const float boatSpeed = 70.0f;
const float boatCornering = 0.1f;
const float boatFriction = 0.01f;

const float heliSpeed = 55.0f;
const float heliCornering = 0.05f;
const float heliFriction = 0.08f;

const char * const bp_names[] = {
    "Jeep Schematics",
    "Tank Schematics",
    "Boat Schematics",
    "Helicopter Schematics",
    "Gemini Gun [Jeep]",
    "Tactical Armor [Soldier]",
    "Ninja Shoes [Soldier]",
    "Spreader Shot [Jeep]",
    "Multinade [Jeep]",
    "Cluster Shot [Heli]",
};
#endif // CONSTANTS_H
