#pragma once

#include "game/utilities/vec.h"
#include "game/entities/enemymech.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"
#include "game/maps/spawnpoint.h"

const SpawnPoint points[] = {
	SpawnPoint({28, 6}, spawnShop<25,5,24,0,22,5>),
	SpawnPoint({17, 16}, Pickups::spawnHackingKit),
	SpawnPoint({45, 16}, spawnDoorA<44,15,1,3>),
	SpawnPoint({43, 10}, spawnBarracks<42,7,4,3>),
	SpawnPoint({35, 15}, spawnBarracks<34,12,3,3>),
	SpawnPoint({64, 12}, spawnBarracks<63,9,3,3>),
	SpawnPoint({70, 14}, spawnBarracks<69,11,3,3>),
	SpawnPoint({70, 22}, spawnBarracks<69,19,3,3>),
	SpawnPoint({21, 16}, spawnDoorNone<20,15,1,3>),
	SpawnPoint({48, 16}, Pickups::spawnKeycardA),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);
