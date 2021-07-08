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
	SpawnPoint({211, 127}, Pickups::spawnKeycardA),
	SpawnPoint({186, 93}, Pickups::spawnKeycardC),
	SpawnPoint({204, 15}, Pickups::spawnKeycardC),
	SpawnPoint({186, 31}, Pickups::spawnKeycardA),
	SpawnPoint({28, 128}, Pickups::spawnKeycardC),
	SpawnPoint({119, 89}, Pickups::spawnKeycardB),
	SpawnPoint({128, 69}, Pickups::spawnKeycardB),
	SpawnPoint({189, 113}, Pickups::spawnKeycardA),
	SpawnPoint({168, 45}, spawnDoorNone<167,38,3,6>),
	SpawnPoint({188, 82}, spawnDoorNone<189,77,3,3>),
	SpawnPoint({218, 43}, spawnDoorNone<215,40,3,3>),
	SpawnPoint({35, 117}, spawnDoorNone<36,114,3,3>),
	SpawnPoint({57, 58}, spawnDoorA<56,59,3,3>),
	SpawnPoint({76, 178}, spawnDoorA<75,179,3,3>),
	SpawnPoint({168, 18}, spawnDoorA<169,15,3,3>),
	SpawnPoint({45, 118}, spawnDoorB<42,117,3,3>),
	SpawnPoint({39, 162}, spawnDoorB<37,159,3,3>),
	SpawnPoint({54, 123}, spawnDoorC<53,124,3,3>),
	SpawnPoint({91, 216}, spawnDoorC<92,215,3,3>),
	SpawnPoint({189, 30}, spawnDoorC<190,29,3,3>),
	SpawnPoint({17, 37}, Pickups::spawnHackingKit),
	SpawnPoint({25, 43}, spawnShop<22,42,21,37,19,42>),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);
