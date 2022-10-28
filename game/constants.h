#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

const float uiEasingTime = 0.2f;
const uint8_t uiEasingTimeMs = uint8_t(uiEasingTime * 1000);

constexpr float physicsTimestep = 0.028f;
const uint8_t physicsTimestepMs = uint8_t(physicsTimestep * 1000);

constexpr int asCounts(float time) {
    return time / physicsTimestep;
}

const float baseTextSpeedLPS = 25.0f;
const float cameraCutsceneSpeed = 12.0f;

constexpr int soldierRepairCost = 4;
constexpr uint8_t repairCosts[] = { 6, 9, 12, 15 };
constexpr uint8_t buildCosts[] = { 10, 15, 20, 25 };

const int barracksMaxMoneyDrops = 5;

const float enemyHeliSpeed = 20.0f;
const float enemyTankSpeed = 12.0f;
const float enemyBoatSpeed = 28.0f;

const int playerStartTileX = 6;//18;
const int playerStartTileY = 16;//33;

const int playerIframeLength = 8;

const int8_t soldierHealth = 8;
const int8_t soldierHealthArmored = 11;
const int8_t helicopterHealth = 16;
const int8_t helicopterHealthArmored = 20;

const float soldierSpeed = 20.0f;

constexpr float jeepSpeed = 50.0f * 1.2f;
const float jeepGrassSpeedFraction = 0.64f;
const float jeepCornering = 0.1f;
const float jeepFriction = 0.05f;

constexpr float tankSpeed = 32.0f * 1.15f;
const float tankCornering = 0.06f;

const float boatSpeed = 70.0f;
const float boatCornering = 0.1f;
const float boatFriction = 0.01f;

const float heliSpeed = 55.0f;
const float heliCornering = 0.05f;
const float heliFriction = 0.08f;

const int doors_count = 25;
const int specials_count = 320;

const char * const bp_names[] = {
    "Jeep Schematics",
    "Tank Schematics",
    "Boat Schematics",
    "Helicopter Schematics",
    "Spreadshot Gun",
    "Tactical Armor",
    "Ninja Shoes",
    "Flamethrower",
    "Multinade",
    "Cluster Shot",
    "All Terrain Tires",
    "Unobtainium Hull",
    "Machine Gun",
    "Counter Motor",
};

const char * const bp_descs[] = {
    "Unlock Vehicle",
    "Unlock Vehicle",
    "Unlock Vehicle",
    "Unlock Vehicle",
    "Unlock Weapon [Jeep]",
    "Health Bonus [Soldier]",
    "Improved Sprint [Soldier]",
    "Unlock Weapon [Tank]",
    "Unlock Weapon [Jeep]",
    "Unlock Weapon [Heli]",
    "Ignore Grass [Jeep]",
    "Improved Health [Heli]",
    "Unlock Weapon",
    "Reduce Drift [Boat]"
};

const int bp_costs[] = {
    0, // jeep
    0, // tank
    0, // boat
    0, // heli
    1, // spreader
    1, // tactical armor
    1, // ninja shoes
    1, // flamethrower
    1, // multinade
    1, // cluster shot
    1, // mud tires
    1, // unobtainium
    1, // Machine Gun
    1, // Counter Motor
};
#endif // CONSTANTS_H
