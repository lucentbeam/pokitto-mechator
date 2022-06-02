#pragma once

#include "core/utilities/vec.h"
#include "game/entities/enemy.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"
#include "game/maps/spawnpoint.h"

const SpawnPoint doors[] = {
	SpawnPoint({44, 16}, spawnDoorA<43,15,1,3,184>),
	SpawnPoint({56, 58}, spawnDoorA<54,59,5,1,184>),
	SpawnPoint({58, 158}, spawnDoorA<57,159,3,3,184>),
	SpawnPoint({107, 20}, spawnDoorA<106,19,3,1,184>),
	SpawnPoint({27, 113}, spawnDoorB<26,110,3,3,184>),
	SpawnPoint({45, 154}, spawnDoorB<42,153,3,3,184>),
	SpawnPoint({58, 123}, spawnDoorC<57,124,3,3,184>),
	SpawnPoint({96, 181}, spawnDoorC<97,180,7,3,184>),
	SpawnPoint({140, 25}, spawnDoorC<141,24,3,3,184>),
	SpawnPoint({241, 100}, spawnDoorC<241,92,1,4,19>),
	SpawnPoint({216, -52}, spawnDoorB<207,-49,7,2,19>),
	SpawnPoint({263, -38}, spawnDoorA<262,-40,1,5,19>),
};
const int door_count = sizeof(doors)/sizeof(SpawnPoint);

