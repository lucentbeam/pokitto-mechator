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

const int8_t barracksMaxLife = 15;

const float baseTextSpeedLPS = 25.0f;
const float cameraCutsceneSpeed = 12.0f;

constexpr int soldierRepairCost = 4;
constexpr uint8_t repairCosts[] = { 4, 6, 8, 10 };
constexpr uint8_t buildCosts[] = { 8, 12, 14, 16 };

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

constexpr float jeepSpeed = 60.0f;
const float jeepGrassSpeedFraction = 0.64f;
const float jeepCornering = 0.1f;
const float jeepFriction = 0.05f;

constexpr float tankSpeed = 37.0f;
const float tankCornering = 0.06f;
constexpr int tankReflectChance = 50;
constexpr float tankSprintWait = 2.0f;

const float boatSpeed = 70.0f;
const float boatCornering = 0.1f;
const float boatFriction = 0.01f;

const float heliSpeed = 55.0f;
const float heliCornering = 0.05f;
const float heliFriction = 0.08f;

const int doors_count = 30;
const int specials_count = 50;

const char * const bp_names[] = {
    "Jeep Schematics",
    "Tank Schematics",
    "Boat Schematics",
    "Helicopter Schematics",

    "Ninja Shoes",
    "Tactical Armor",

    "Spreadshot Gun",

    "Multinade",
    "Flamethrower",
    "All Terrain Tires",
    "Machine Gun",
    "Counter Motor",
    "Unobtainium Hull",
    "Cluster Shot",
    "Shotgun",
    "Reflective Armor",
    "Armor Plating",
    "Power Rerouting",
    "Stimpack",
    "Rear Mounted Gun",
};

const char * const bp_descs[] = {
    "Mobile Ground Vehicle",
    "Durable Ground Vehicle",
    "Fast Water Vehicle",
    "Free as a Birb",

    "Improved Sprint",
    "Health Bonus",

    "Three Shots in One",
    "Fast Grenades",
    "Lots of Flames",
    "Ignore Grass",
    "Very Fast Bullets",
    "Reduce Drift",

    "Health Bonus",
    "Lots of Missiles",
    "Multiple Bullets",

    "May Reflect Bullets",
    "Damage Free Runovers",

    "Automatic Sprinting",
    "Fire While Sprinting",
    "Bonus Fire Backwards"
};

const char * const bp_desc_types[] = {
    "Jeep Unlock",
    "Tank Unlock",
    "Boat Unlock",
    "Helicopter Unlock",

    "Soldier Passive",
    "Soldier Passive",

    "Jeep Weapon",
    "Jeep Weapon",
    "Tank Weapon",
    "Jeep Passive",
    "Soldier/Tank/Boat Weapon",
    "Boat Passive",

    "Helicopter Passive",
    "Helicopter Weapon",
    "Soldier Weapon",

    "Tank Passive",
    "Jeep Passive",

    "Tank Passive",
    "Soldier Ability",
    "Jeep Ability"
};

const int bp_costs[] = {
    0, // jeep
    0, // tank
    0, // boat
    0, // heli
    8, // ninja shoes
    8, // tactical armor
    8, // spreader
    8, // multinade
    16, // flamethrower
    8, // mud tires
    16, // Machine Gun
    10, // Counter Motor
    30, // unobtainium
    20, // cluster shot
    12, // shotgun
    16, // reflective hull
    14, // armor
    14, // tank speed,
    12, // stimpack
    8,  // rear gun
};

const uint8_t bullet_empty[] = {6, 6, 1, 1, 0, 0, 1, 1,
                                      1, 0, 0, 0, 0,1,
                                      0, 0, 0, 0, 0,0,
                                      0, 0, 0, 0, 0,0,
                                      1, 0, 0, 0, 0,1,
                                      1, 1, 0, 0, 1, 1,};
#endif // CONSTANTS_H
