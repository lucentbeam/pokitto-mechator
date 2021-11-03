#pragma once

#include "core/utilities/vec.h"
#include "game/entities/enemymech.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"
#include "game/maps/spawnpoint.h"

const SpawnPoint points[] = {
	SpawnPoint({27, 6}, spawnShop<24,5,23,0,21,5>),
	SpawnPoint({16, 16}, Pickups::spawnHackingKit),
	SpawnPoint({44, 16}, spawnDoorA<43,15,1,3>),
	SpawnPoint({42, 10}, spawnBarracks<41,7,4,3>),
	SpawnPoint({34, 15}, spawnBarracks<33,12,3,3>),
	SpawnPoint({63, 12}, spawnBarracks<62,9,3,3>),
	SpawnPoint({69, 14}, spawnBarracks<68,11,3,3>),
	SpawnPoint({69, 22}, spawnBarracks<68,19,3,3>),
	SpawnPoint({20, 16}, spawnDoorNone<19,15,1,3>),
	SpawnPoint({47, 16}, Pickups::spawnKeycardA),
	SpawnPoint({210, 127}, Pickups::spawnKeycardA),
	SpawnPoint({162, 86}, Pickups::spawnKeycardC),
	SpawnPoint({203, 15}, Pickups::spawnKeycardC),
	SpawnPoint({185, 31}, Pickups::spawnKeycardA),
	SpawnPoint({27, 128}, Pickups::spawnKeycardC),
	SpawnPoint({121, 83}, Pickups::spawnKeycardB),
	SpawnPoint({127, 69}, Pickups::spawnKeycardB),
	SpawnPoint({188, 113}, Pickups::spawnKeycardA),
	SpawnPoint({167, 45}, spawnDoorNone<166,38,3,6>),
	SpawnPoint({169, 83}, spawnDoorNone<171,78,2,9>),
	SpawnPoint({217, 43}, spawnDoorNone<214,40,3,3>),
	SpawnPoint({34, 117}, spawnDoorNone<35,114,3,3>),
	SpawnPoint({56, 58}, spawnDoorA<54,59,5,1>),
	SpawnPoint({75, 178}, spawnDoorA<74,179,3,3>),
	SpawnPoint({167, 18}, spawnDoorA<168,15,3,3>),
	SpawnPoint({44, 118}, spawnDoorB<41,117,3,3>),
	SpawnPoint({38, 162}, spawnDoorB<36,159,3,3>),
	SpawnPoint({53, 123}, spawnDoorC<52,124,3,3>),
	SpawnPoint({90, 216}, spawnDoorC<91,215,3,3>),
	SpawnPoint({188, 30}, spawnDoorC<189,29,3,3>),
	SpawnPoint({16, 37}, Pickups::spawnHackingKit),
	SpawnPoint({24, 43}, spawnShop<21,42,20,37,18,42>),
	SpawnPoint({79, 66}, spawnBarracks<78,63,3,3>),
	SpawnPoint({94, 66}, spawnBarracks<93,63,3,3>),
	SpawnPoint({107, 86}, spawnBarracks<106,83,3,3>),
	SpawnPoint({96, 79}, spawnBarracks<95,74,5,5>),
	SpawnPoint({111, 80}, spawnDoorNone<116,84,1,3>),
	SpawnPoint({82, 82}, spawnDoorNone<83,74,1,1>),
	SpawnPoint({82, 82}, spawnDoorNone<87,84,1,3>),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);
