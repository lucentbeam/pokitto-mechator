#pragma once

#include "core/utilities/vec.h"
#include "game/entities/enemy.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"
#include "game/maps/spawnpoint.h"

const SpawnPoint points[] = {
	SpawnPoint({27, 6}, spawnShop<24,5,23,0,21,5>),
	SpawnPoint({16, 16}, Pickups::spawnHackingKit),
	SpawnPoint({42, 10}, spawnBarracks<41,7,4,3>),
	SpawnPoint({34, 15}, spawnBarracks<33,12,3,3>),
	SpawnPoint({55, 13}, spawnBarracks<54,10,3,3>),
	SpawnPoint({69, 22}, spawnBarracks<68,19,3,3>),
	SpawnPoint({20, 16}, spawnDoorNone<19,15,1,3>),
	SpawnPoint({69, 20}, Pickups::spawnKeycardA),
	SpawnPoint({200, 142}, Pickups::spawnKeycardA),
	SpawnPoint({162, 86}, Pickups::spawnKeycardC),
	SpawnPoint({123, 10}, Pickups::spawnKeycardC),
	SpawnPoint({135, 20}, Pickups::spawnKeycardA),
	SpawnPoint({38, 129}, Pickups::spawnKeycardC),
	SpawnPoint({121, 83}, Pickups::spawnKeycardB),
	SpawnPoint({136, 66}, Pickups::spawnKeycardB),
	SpawnPoint({169, 114}, Pickups::spawnKeycardA),
	SpawnPoint({170, 83}, spawnDoorNone<191,85,9,3>),
	SpawnPoint({179, 28}, spawnDoorNone<178,26,1,5>),
	SpawnPoint({13, 127}, spawnDoorNone<12,124,3,3>),
	SpawnPoint({14, 49}, Pickups::spawnHackingKit),
	SpawnPoint({13, 49}, spawnShop<10,48,9,43,7,48>),
	SpawnPoint({79, 66}, spawnBarracks<78,63,3,3>),
	SpawnPoint({94, 66}, spawnBarracks<93,63,3,3>),
	SpawnPoint({107, 86}, spawnBarracks<106,83,3,3>),
	SpawnPoint({96, 79}, spawnBarracks<95,74,5,5>),
	SpawnPoint({111, 80}, spawnDoorNone<116,84,1,3>),
	SpawnPoint({159, 47}, spawnDoorNone<153,54,5,3>),
	SpawnPoint({139, 139}, spawnDoorNone<159,143,3,3>),
	SpawnPoint({28, 99}, Pickups::spawnBlueprint),
	SpawnPoint({161, 209}, Pickups::spawnBlueprint),
	SpawnPoint({166, 201}, spawnDoorNone<104,39,3,8>),
	SpawnPoint({23, 41}, Enemy::spawnMine),
	SpawnPoint({16, 27}, Enemy::spawnBomber),
	SpawnPoint({17, 53}, Enemy::spawnTank),
	SpawnPoint({30, 41}, Enemy::spawnTurret),
	SpawnPoint({28, 48}, Enemy::spawnTank),
	SpawnPoint({59, 57}, Enemy::spawnMech),
	SpawnPoint({53, 57}, Enemy::spawnMech),
	SpawnPoint({72, 43}, Enemy::spawnTurret),
	SpawnPoint({127, 63}, Enemy::spawnTurret),
	SpawnPoint({139, 71}, Enemy::spawnTurret),
	SpawnPoint({101, 62}, Enemy::spawnMine),
	SpawnPoint({105, 60}, Enemy::spawnMine),
	SpawnPoint({111, 61}, Enemy::spawnMine),
	SpawnPoint({116, 60}, Enemy::spawnMine),
	SpawnPoint({86, 53}, Enemy::spawnTank),
	SpawnPoint({87, 44}, Enemy::spawnMech),
	SpawnPoint({96, 52}, Enemy::spawnMech),
	SpawnPoint({115, 53}, Enemy::spawnBomber),
	SpawnPoint({141, 53}, Enemy::spawnBomber),
	SpawnPoint({132, 49}, Enemy::spawnMech),
	SpawnPoint({130, 54}, Enemy::spawnMech),
	SpawnPoint({23, 77}, Enemy::spawnMine),
	SpawnPoint({19, 82}, Enemy::spawnMine),
	SpawnPoint({29, 81}, Enemy::spawnMine),
	SpawnPoint({30, 76}, Enemy::spawnMine),
	SpawnPoint({37, 75}, Enemy::spawnMine),
	SpawnPoint({42, 72}, Enemy::spawnMine),
	SpawnPoint({48, 75}, Enemy::spawnMine),
	SpawnPoint({47, 79}, Enemy::spawnMine),
	SpawnPoint({137, 81}, Enemy::spawnTurret),
	SpawnPoint({147, 85}, Enemy::spawnTurret),
	SpawnPoint({157, 81}, Enemy::spawnTurret),
	SpawnPoint({48, 60}, Enemy::spawnTurret),
	SpawnPoint({58, 69}, Enemy::spawnTurret),
	SpawnPoint({25, 67}, Enemy::spawnTank),
	SpawnPoint({34, 66}, Enemy::spawnTank),
	SpawnPoint({9, 60}, Enemy::spawnTurret),
	SpawnPoint({83, 84}, spawnVerticalLasers< 83, 4>),
	SpawnPoint({27, 116}, spawnHorizontalLasers< 25, 5>),
	SpawnPoint({24, 119}, spawnVerticalLasers< 117, 5>),
	SpawnPoint({30, 119}, spawnVerticalLasers< 117, 5>),
	SpawnPoint({156, 148}, spawnVerticalLasers< 146, 4>),
	SpawnPoint({150, 139}, spawnVerticalLasers< 135, 9>),
	SpawnPoint({204, 137}, spawnHorizontalLasers< 202, 4>),
	SpawnPoint({55, 143}, spawnVerticalLasers< 142, 4>),
	SpawnPoint({59, 144}, spawnVerticalLasers< 142, 4>),
	SpawnPoint({63, 144}, spawnVerticalLasers< 142, 4>),
	SpawnPoint({110, 84}, spawnVerticalLasers< 83, 4>),
	SpawnPoint({98, 11}, spawnHorizontalLasers< 96, 6>),
	SpawnPoint({210, 30}, Enemy::spawnTurret),
	SpawnPoint({142, 22}, Enemy::spawnTurret),
	SpawnPoint({142, 28}, Enemy::spawnTurret),
	SpawnPoint({19, 94}, Enemy::spawnTurretDisabled),
	SpawnPoint({19, 105}, Enemy::spawnTurretDisabled),
	SpawnPoint({35, 105}, Enemy::spawnTurretDisabled),
	SpawnPoint({35, 94}, Enemy::spawnTurretDisabled),
	SpawnPoint({64, 153}, Enemy::spawnTurret),
	SpawnPoint({70, 175}, Enemy::spawnTurret),
	SpawnPoint({169, 201}, Enemy::spawnTurret),
	SpawnPoint({148, 160}, Enemy::spawnTurret),
	SpawnPoint({190, 103}, Enemy::spawnTurret),
	SpawnPoint({206, 101}, Enemy::spawnTurret),
	SpawnPoint({203, 122}, spawnBarracks<202,119,3,3>),
	SpawnPoint({171, 125}, spawnBarracks<170,122,3,3>),
	SpawnPoint({52, 153}, spawnBarracks<51,154,4,5>),
	SpawnPoint({156, 128}, Enemy::spawnMine),
	SpawnPoint({159, 130}, Enemy::spawnMine),
	SpawnPoint({163, 128}, Enemy::spawnMine),
	SpawnPoint({162, 132}, Enemy::spawnMine),
	SpawnPoint({151, 8}, Enemy::spawnMine),
	SpawnPoint({158, 8}, Enemy::spawnMine),
	SpawnPoint({161, 12}, Enemy::spawnMine),
	SpawnPoint({163, 15}, Enemy::spawnMine),
	SpawnPoint({166, 9}, Enemy::spawnMine),
	SpawnPoint({174, 14}, Enemy::spawnMine),
	SpawnPoint({177, 17}, Enemy::spawnMine),
	SpawnPoint({179, 10}, Enemy::spawnMine),
	SpawnPoint({186, 11}, Enemy::spawnMine),
	SpawnPoint({185, 15}, Enemy::spawnMine),
	SpawnPoint({192, 13}, Enemy::spawnMine),
	SpawnPoint({194, 18}, Enemy::spawnMine),
	SpawnPoint({31, 138}, Enemy::spawnMine),
	SpawnPoint({37, 143}, Enemy::spawnMine),
	SpawnPoint({27, 143}, Enemy::spawnMine),
	SpawnPoint({24, 135}, Enemy::spawnMine),
	SpawnPoint({16, 130}, Enemy::spawnMine),
	SpawnPoint({23, 128}, Enemy::spawnMine),
	SpawnPoint({204, 83}, Enemy::spawnBomber),
	SpawnPoint({163, 99}, Enemy::spawnBomber),
	SpawnPoint({91, 121}, Enemy::spawnBomber),
	SpawnPoint({149, 111}, Enemy::spawnBomber),
	SpawnPoint({102, 95}, Enemy::spawnBomber),
	SpawnPoint({119, 192}, spawnDoorNone<124,192,5,1>),
	SpawnPoint({173, 164}, Enemy::spawnTurret),
	SpawnPoint({207, 147}, Enemy::spawnTurret),
	SpawnPoint({67, 93}, Enemy::spawnTank),
	SpawnPoint({53, 101}, Enemy::spawnTank),
	SpawnPoint({57, 114}, Enemy::spawnTank),
	SpawnPoint({66, 120}, Enemy::spawnTank),
	SpawnPoint({75, 106}, Enemy::spawnTank),
	SpawnPoint({25, 51}, Enemy::spawnMech),
	SpawnPoint({66, 110}, Enemy::spawnMech),
	SpawnPoint({70, 115}, Enemy::spawnMech),
	SpawnPoint({57, 119}, Enemy::spawnMech),
	SpawnPoint({71, 104}, Enemy::spawnMech),
	SpawnPoint({81, 97}, Enemy::spawnMech),
	SpawnPoint({117, 92}, Enemy::spawnMech),
	SpawnPoint({130, 97}, Enemy::spawnMech),
	SpawnPoint({126, 102}, Enemy::spawnMech),
	SpawnPoint({117, 106}, Enemy::spawnMech),
	SpawnPoint({103, 119}, Enemy::spawnMech),
	SpawnPoint({111, 113}, Enemy::spawnMech),
	SpawnPoint({148, 125}, Enemy::spawnMech),
	SpawnPoint({133, 130}, Enemy::spawnMech),
	SpawnPoint({159, 138}, Enemy::spawnMech),
	SpawnPoint({167, 135}, Enemy::spawnMech),
	SpawnPoint({147, 149}, Enemy::spawnMech),
	SpawnPoint({168, 156}, Enemy::spawnMech),
	SpawnPoint({195, 96}, Enemy::spawnMech),
	SpawnPoint({200, 95}, Enemy::spawnMech),
	SpawnPoint({198, 97}, Enemy::spawnMech),
	SpawnPoint({190, 71}, Enemy::spawnMech),
	SpawnPoint({174, 84}, Enemy::spawnMech),
	SpawnPoint({190, 74}, Enemy::spawnMech),
	SpawnPoint({171, 55}, Enemy::spawnMech),
	SpawnPoint({179, 68}, Enemy::spawnMech),
	SpawnPoint({108, 118}, Enemy::spawnTank),
	SpawnPoint({142, 128}, Enemy::spawnTank),
	SpawnPoint({154, 123}, Enemy::spawnTurret),
	SpawnPoint({161, 122}, Enemy::spawnTank),
	SpawnPoint({146, 99}, Enemy::spawnMech),
	SpawnPoint({155, 91}, Enemy::spawnMech),
	SpawnPoint({178, 98}, Enemy::spawnMech),
	SpawnPoint({180, 55}, Enemy::spawnTank),
	SpawnPoint({173, 64}, Enemy::spawnTank),
	SpawnPoint({147, 48}, Enemy::spawnTank),
	SpawnPoint({62, 64}, Enemy::spawnTank),
	SpawnPoint({73, 78}, Enemy::spawnTank),
	SpawnPoint({67, 79}, Enemy::spawnTank),
	SpawnPoint({77, 72}, Enemy::spawnHelicopter),
	SpawnPoint({132, 67}, Enemy::spawnHelicopter),
	SpawnPoint({188, 26}, Enemy::spawnHelicopter),
	SpawnPoint({107, 14}, Enemy::spawnHelicopter),
	SpawnPoint({43, 131}, Enemy::spawnHelicopter),
	SpawnPoint({92, 178}, Enemy::spawnTank),
	SpawnPoint({117, 201}, Enemy::spawnTank),
	SpawnPoint({128, 196}, Enemy::spawnMech),
	SpawnPoint({128, 202}, Enemy::spawnMech),
	SpawnPoint({137, 201}, Enemy::spawnMech),
	SpawnPoint({105, 191}, Enemy::spawnMech),
	SpawnPoint({115, 181}, Enemy::spawnMech),
	SpawnPoint({74, 181}, Enemy::spawnMech),
	SpawnPoint({82, 171}, Enemy::spawnMech),
	SpawnPoint({64, 166}, Enemy::spawnMech),
	SpawnPoint({53, 174}, Enemy::spawnMech),
	SpawnPoint({31, 163}, Enemy::spawnMech),
	SpawnPoint({30, 153}, Enemy::spawnMech),
	SpawnPoint({33, 157}, Enemy::spawnMech),
	SpawnPoint({37, 153}, Enemy::spawnMech),
	SpawnPoint({20, 134}, Enemy::spawnMech),
	SpawnPoint({17, 119}, Enemy::spawnMech),
	SpawnPoint({120, 10}, Enemy::spawnMech),
	SpawnPoint({126, 10}, Enemy::spawnMech),
	SpawnPoint({123, 16}, Enemy::spawnMech),
	SpawnPoint({102, 16}, Enemy::spawnMech),
	SpawnPoint({106, 30}, Enemy::spawnTank),
	SpawnPoint({126, 27}, Enemy::spawnTank),
	SpawnPoint({172, 31}, Enemy::spawnTank),
	SpawnPoint({207, 22}, Enemy::spawnTank),
	SpawnPoint({203, 24}, Enemy::spawnMech),
	SpawnPoint({210, 24}, Enemy::spawnMech),
	SpawnPoint({184, 132}, Enemy::spawnTank),
	SpawnPoint({187, 145}, Enemy::spawnTank),
	SpawnPoint({188, 125}, Enemy::spawnTank),
	SpawnPoint({119, 124}, Enemy::spawnBomber),
	SpawnPoint({15, 66}, Enemy::spawnMech),
	SpawnPoint({28, 63}, Enemy::spawnMech),
	SpawnPoint({30, 2}, Pickups::spawnBlueprint),
	SpawnPoint({30, 3}, Enemy::spawnMech),
	SpawnPoint({25, 4}, spawnDoorNone<29,5,2,1>),
	SpawnPoint({27, 93}, spawnShop<30,92,23,87,21,92>),
	SpawnPoint({26, 99}, Pickups::spawnHackingKit),
	SpawnPoint({35, 118}, Pickups::spawnKeycardB),
	SpawnPoint({27, 97}, spawnBarracks<26,98,3,4>),
	SpawnPoint({23, 99}, spawnBarracks<24,98,2,4>),
	SpawnPoint({31, 99}, spawnBarracks<29,98,2,4>),
	SpawnPoint({37, 46}, spawnBarracks<36,42,3,4>),
	SpawnPoint({37, 43}, Pickups::spawnBlueprint),
	SpawnPoint({55, 48}, Enemy::spawnMech),
	SpawnPoint({72, 45}, Enemy::spawnMech),
	SpawnPoint({68, 52}, Enemy::spawnMech),
	SpawnPoint({96, 48}, Enemy::spawnMine),
	SpawnPoint({101, 52}, Enemy::spawnMine),
	SpawnPoint({104, 55}, Enemy::spawnMine),
	SpawnPoint({106, 50}, Enemy::spawnMine),
	SpawnPoint({159, 69}, Pickups::spawnBlueprint),
	SpawnPoint({174, 81}, Enemy::spawnMech),
	SpawnPoint({211, 112}, Pickups::spawnBlueprint),
	SpawnPoint({62, 133}, spawnVerticalLasers< 132, 4>),
	SpawnPoint({54, 129}, Enemy::spawnMech),
	SpawnPoint({62, 129}, Enemy::spawnMech),
	SpawnPoint({55, 136}, Enemy::spawnTank),
	SpawnPoint({64, 148}, Enemy::spawnTank),
	SpawnPoint({67, 157}, Enemy::spawnTank),
	SpawnPoint({34, 145}, Enemy::spawnMech),
	SpawnPoint({117, 9}, spawnBarracks<116,6,3,3>),
	SpawnPoint({111, 7}, Enemy::spawnMech),
	SpawnPoint({139, 4}, Enemy::spawnMech),
	SpawnPoint({133, 7}, Enemy::spawnMech),
	SpawnPoint({124, 24}, Enemy::spawnTurret),
	SpawnPoint({124, 34}, Enemy::spawnTurret),
	SpawnPoint({103, 28}, Enemy::spawnTurret),
	SpawnPoint({154, 25}, Enemy::spawnTurret),
	SpawnPoint({161, 33}, Enemy::spawnTurret),
	SpawnPoint({172, 26}, Enemy::spawnTurret),
	SpawnPoint({34, 46}, spawnBarracks<33,42,3,4>),
	SpawnPoint({77, 166}, Pickups::spawnHackingKit),
	SpawnPoint({76, 166}, spawnShop<73,165,72,160,70,165>),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);

