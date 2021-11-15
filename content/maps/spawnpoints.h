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
	SpawnPoint({202, 141}, Pickups::spawnKeycardA),
	SpawnPoint({162, 86}, Pickups::spawnKeycardC),
	SpawnPoint({121, 12}, Pickups::spawnKeycardC),
	SpawnPoint({130, 25}, Pickups::spawnKeycardA),
	SpawnPoint({13, 129}, Pickups::spawnKeycardC),
	SpawnPoint({121, 83}, Pickups::spawnKeycardB),
	SpawnPoint({136, 66}, Pickups::spawnKeycardB),
	SpawnPoint({169, 114}, Pickups::spawnKeycardA),
	SpawnPoint({169, 83}, spawnDoorNone<171,78,2,9>),
	SpawnPoint({180, 28}, spawnDoorNone<177,25,3,3>),
	SpawnPoint({13, 127}, spawnDoorNone<12,124,3,3>),
	SpawnPoint({56, 58}, spawnDoorA<54,59,5,1>),
	SpawnPoint({58, 158}, spawnDoorA<57,159,3,3>),
	SpawnPoint({98, 18}, spawnDoorA<99,15,3,3>),
	SpawnPoint({25, 115}, spawnDoorB<24,112,3,3>),
	SpawnPoint({45, 154}, spawnDoorB<42,153,3,3>),
	SpawnPoint({58, 123}, spawnDoorC<57,124,3,3>),
	SpawnPoint({96, 181}, spawnDoorC<97,180,7,3>),
	SpawnPoint({133, 24}, spawnDoorC<134,23,3,3>),
	SpawnPoint({16, 37}, Pickups::spawnHackingKit),
	SpawnPoint({24, 43}, spawnShop<21,42,20,37,18,42>),
	SpawnPoint({79, 66}, spawnBarracks<78,63,3,3>),
	SpawnPoint({94, 66}, spawnBarracks<93,63,3,3>),
	SpawnPoint({107, 86}, spawnBarracks<106,83,3,3>),
	SpawnPoint({96, 79}, spawnBarracks<95,74,5,5>),
	SpawnPoint({111, 80}, spawnDoorNone<116,84,1,3>),
	SpawnPoint({159, 47}, spawnDoorNone<153,54,5,3>),
	SpawnPoint({139, 139}, spawnDoorNone<159,143,3,3>),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);
