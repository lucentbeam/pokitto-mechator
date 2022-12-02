#pragma once

#include "core/utilities/vec.h"
#include "game/entities/enemy.h"
#include "game/entities/barracks.h"
#include "game/entities/pickups.h"
#include "game/entities/pois.h"
#include "game/maps/spawnpoint.h"

const SpawnPoint points[] = {
	SpawnPoint({27, 6}, SpawnPoint::Shop),
	SpawnPoint({44, 16}, SpawnPoint::DoorA),
	SpawnPoint({42, 10}, SpawnPoint::EnemyBarracks),
	SpawnPoint({34, 15}, SpawnPoint::EnemyBarracks),
	SpawnPoint({55, 13}, SpawnPoint::EnemyBarracks),
	SpawnPoint({69, 22}, SpawnPoint::EnemyBarracks),
	SpawnPoint({20, 16}, SpawnPoint::DoorNone),
	SpawnPoint({170, 83}, SpawnPoint::DoorNone),
	SpawnPoint({179, 28}, SpawnPoint::DoorNone),
	SpawnPoint({13, 127}, SpawnPoint::DoorC),
	SpawnPoint({56, 58}, SpawnPoint::DoorA),
	SpawnPoint({58, 158}, SpawnPoint::DoorA),
	SpawnPoint({107, 20}, SpawnPoint::DoorA),
	SpawnPoint({27, 113}, SpawnPoint::DoorB),
	SpawnPoint({45, 154}, SpawnPoint::DoorB),
	SpawnPoint({58, 123}, SpawnPoint::DoorC),
	SpawnPoint({96, 181}, SpawnPoint::DoorC),
	SpawnPoint({140, 25}, SpawnPoint::DoorC),
	SpawnPoint({81, 63}, SpawnPoint::EnemyBarracks),
	SpawnPoint({107, 86}, SpawnPoint::EnemyBarracks),
	SpawnPoint({96, 79}, SpawnPoint::EnemyBarracks),
	SpawnPoint({111, 80}, SpawnPoint::DoorNone),
	SpawnPoint({159, 49}, SpawnPoint::DoorNone),
	SpawnPoint({139, 139}, SpawnPoint::DoorNone),
	SpawnPoint({166, 201}, SpawnPoint::DoorNone),
	SpawnPoint({83, 83}, SpawnPoint::LasersVert),
	SpawnPoint({25, 116}, SpawnPoint::LasersHoriz),
	SpawnPoint({24, 117}, SpawnPoint::LasersVert),
	SpawnPoint({30, 117}, SpawnPoint::LasersVert),
	SpawnPoint({156, 146}, SpawnPoint::LasersVert),
	SpawnPoint({150, 135}, SpawnPoint::LasersVert),
	SpawnPoint({202, 137}, SpawnPoint::LasersHoriz),
	SpawnPoint({55, 142}, SpawnPoint::LasersVert),
	SpawnPoint({59, 142}, SpawnPoint::LasersVert),
	SpawnPoint({63, 142}, SpawnPoint::LasersVert),
	SpawnPoint({110, 83}, SpawnPoint::LasersVert),
	SpawnPoint({96, 11}, SpawnPoint::LasersHoriz),
	SpawnPoint({203, 122}, SpawnPoint::EnemyBarracks),
	SpawnPoint({171, 125}, SpawnPoint::EnemyBarracks),
	SpawnPoint({52, 153}, SpawnPoint::EnemyBarracks),
	SpawnPoint({119, 192}, SpawnPoint::DoorNone),
	SpawnPoint({25, 4}, SpawnPoint::DoorNone),
	SpawnPoint({27, 93}, SpawnPoint::Shop),
	SpawnPoint({27, 97}, SpawnPoint::EnemyBarracks),
	SpawnPoint({23, 99}, SpawnPoint::EnemyBarracks),
	SpawnPoint({31, 99}, SpawnPoint::EnemyBarracks),
	SpawnPoint({37, 46}, SpawnPoint::EnemyBarracks),
	SpawnPoint({62, 132}, SpawnPoint::LasersVert),
	SpawnPoint({117, 9}, SpawnPoint::EnemyBarracks),
	SpawnPoint({34, 46}, SpawnPoint::EnemyBarracks),
	SpawnPoint({100, 142}, SpawnPoint::EnemyBarracks),
	SpawnPoint({116, 140}, SpawnPoint::EnemyBarracks),
	SpawnPoint({112, 159}, SpawnPoint::EnemyBarracks),
	SpawnPoint({132, 166}, SpawnPoint::EnemyBarracks),
	SpawnPoint({55, 76}, SpawnPoint::Shop),
	SpawnPoint({72, 45}, SpawnPoint::Shop),
	SpawnPoint({189, 71}, SpawnPoint::Shop),
	SpawnPoint({69, 130}, SpawnPoint::Shop),
	SpawnPoint({163, 142}, SpawnPoint::Shop),
	SpawnPoint({108, 13}, SpawnPoint::Shop),
	SpawnPoint({210, 32}, SpawnPoint::Shop),
	SpawnPoint({133, 94}, SpawnPoint::Shop),
	SpawnPoint({172, 28}, SpawnPoint::Shop),
	SpawnPoint({180, 51}, SpawnPoint::EnemyBarracks),
	SpawnPoint({193, 13}, SpawnPoint::EnemyBarracks),
	SpawnPoint({196, 171}, SpawnPoint::Shop),
	SpawnPoint({45, 197}, SpawnPoint::Shop),
	SpawnPoint({5, 26}, SpawnPoint::Shop),
	SpawnPoint({221, 204}, SpawnPoint::EnemyBarracks),
	SpawnPoint({224, 220}, SpawnPoint::EnemyBarracks),
	SpawnPoint({232, 213}, SpawnPoint::WaterMine),
	SpawnPoint({233, 217}, SpawnPoint::WaterMine),
	SpawnPoint({-36, 47}, SpawnPoint::DoorNone),
	SpawnPoint({-34, 39}, SpawnPoint::LasersVert),
	SpawnPoint({-23, 39}, SpawnPoint::LasersVert),
	SpawnPoint({-17, 59}, SpawnPoint::LasersVert),
	SpawnPoint({-28, 50}, SpawnPoint::LasersVert),
	SpawnPoint({-36, 67}, SpawnPoint::DoorNone),
	SpawnPoint({-34, 58}, SpawnPoint::DoorC),
	SpawnPoint({-13, 55}, SpawnPoint::WaterMine),
	SpawnPoint({-25, 67}, SpawnPoint::WaterMine),
	SpawnPoint({-15, 67}, SpawnPoint::WaterMine),
	SpawnPoint({-13, 59}, SpawnPoint::WaterMine),
	SpawnPoint({-41, 51}, SpawnPoint::WaterMine),
	SpawnPoint({-20, 56}, SpawnPoint::WaterMine),
	SpawnPoint({-30, 194}, SpawnPoint::DoorB),
	SpawnPoint({17, 208}, SpawnPoint::DoorA),
	SpawnPoint({12, 192}, SpawnPoint::EnemyBarracks),
	SpawnPoint({-43, 206}, SpawnPoint::Shop),
	SpawnPoint({14, 220}, SpawnPoint::Shop),
	SpawnPoint({-3, 203}, SpawnPoint::Shop),
	SpawnPoint({-37, 177}, SpawnPoint::LasersVert),
	SpawnPoint({-28, 177}, SpawnPoint::LasersVert),
	SpawnPoint({-45, 191}, SpawnPoint::LasersHoriz),
	SpawnPoint({-27, 184}, SpawnPoint::LasersHoriz),
	SpawnPoint({-42, 178}, SpawnPoint::WaterMine),
	SpawnPoint({-42, 185}, SpawnPoint::WaterMine),
	SpawnPoint({-24, 193}, SpawnPoint::WaterMine),
	SpawnPoint({16, 16}, SpawnPoint::HackingKit),
	SpawnPoint({69, 20}, SpawnPoint::KeyA),
	SpawnPoint({200, 142}, SpawnPoint::KeyA),
	SpawnPoint({166, 81}, SpawnPoint::KeyC),
	SpawnPoint({123, 10}, SpawnPoint::KeyC),
	SpawnPoint({135, 20}, SpawnPoint::KeyA),
	SpawnPoint({38, 129}, SpawnPoint::KeyC),
	SpawnPoint({121, 83}, SpawnPoint::KeyC),
	SpawnPoint({136, 66}, SpawnPoint::KeyB),
	SpawnPoint({169, 114}, SpawnPoint::KeyA),
	SpawnPoint({12, 45}, SpawnPoint::HackingKit),
	SpawnPoint({28, 99}, SpawnPoint::Blueprint),
	SpawnPoint({161, 209}, SpawnPoint::Blueprint),
	SpawnPoint({23, 41}, SpawnPoint::Mine),
	SpawnPoint({16, 27}, SpawnPoint::Bomber),
	SpawnPoint({17, 53}, SpawnPoint::Tank),
	SpawnPoint({30, 41}, SpawnPoint::Turret),
	SpawnPoint({28, 48}, SpawnPoint::Tank),
	SpawnPoint({59, 57}, SpawnPoint::Mech),
	SpawnPoint({53, 57}, SpawnPoint::Mech),
	SpawnPoint({72, 43}, SpawnPoint::Turret),
	SpawnPoint({127, 63}, SpawnPoint::Turret),
	SpawnPoint({139, 71}, SpawnPoint::Turret),
	SpawnPoint({101, 62}, SpawnPoint::Mine),
	SpawnPoint({105, 60}, SpawnPoint::Mine),
	SpawnPoint({111, 61}, SpawnPoint::Mine),
	SpawnPoint({116, 60}, SpawnPoint::Mine),
	SpawnPoint({86, 53}, SpawnPoint::Tank),
	SpawnPoint({87, 44}, SpawnPoint::Mech),
	SpawnPoint({96, 52}, SpawnPoint::Mech),
	SpawnPoint({115, 53}, SpawnPoint::Bomber),
	SpawnPoint({141, 53}, SpawnPoint::Bomber),
	SpawnPoint({132, 49}, SpawnPoint::Mech),
	SpawnPoint({130, 54}, SpawnPoint::Mech),
	SpawnPoint({23, 77}, SpawnPoint::Mine),
	SpawnPoint({19, 82}, SpawnPoint::Mine),
	SpawnPoint({29, 81}, SpawnPoint::Mine),
	SpawnPoint({30, 76}, SpawnPoint::Mine),
	SpawnPoint({37, 75}, SpawnPoint::Mine),
	SpawnPoint({42, 72}, SpawnPoint::Mine),
	SpawnPoint({48, 75}, SpawnPoint::Mine),
	SpawnPoint({47, 79}, SpawnPoint::Mine),
	SpawnPoint({137, 81}, SpawnPoint::Turret),
	SpawnPoint({147, 85}, SpawnPoint::Turret),
	SpawnPoint({157, 81}, SpawnPoint::Turret),
	SpawnPoint({48, 60}, SpawnPoint::Turret),
	SpawnPoint({58, 69}, SpawnPoint::Turret),
	SpawnPoint({25, 67}, SpawnPoint::Tank),
	SpawnPoint({34, 66}, SpawnPoint::Tank),
	SpawnPoint({9, 60}, SpawnPoint::Turret),
	SpawnPoint({210, 30}, SpawnPoint::Turret),
	SpawnPoint({142, 22}, SpawnPoint::Turret),
	SpawnPoint({142, 28}, SpawnPoint::Turret),
	SpawnPoint({19, 94}, SpawnPoint::TurretDisabled),
	SpawnPoint({19, 105}, SpawnPoint::TurretDisabled),
	SpawnPoint({35, 105}, SpawnPoint::TurretDisabled),
	SpawnPoint({35, 94}, SpawnPoint::TurretDisabled),
	SpawnPoint({64, 153}, SpawnPoint::Turret),
	SpawnPoint({70, 175}, SpawnPoint::Turret),
	SpawnPoint({169, 201}, SpawnPoint::Turret),
	SpawnPoint({148, 160}, SpawnPoint::Turret),
	SpawnPoint({190, 103}, SpawnPoint::Turret),
	SpawnPoint({206, 101}, SpawnPoint::Turret),
	SpawnPoint({156, 128}, SpawnPoint::Mine),
	SpawnPoint({159, 130}, SpawnPoint::Mine),
	SpawnPoint({163, 128}, SpawnPoint::Mine),
	SpawnPoint({162, 132}, SpawnPoint::Mine),
	SpawnPoint({151, 8}, SpawnPoint::Mine),
	SpawnPoint({158, 8}, SpawnPoint::Mine),
	SpawnPoint({161, 12}, SpawnPoint::Mine),
	SpawnPoint({163, 15}, SpawnPoint::Mine),
	SpawnPoint({166, 9}, SpawnPoint::Mine),
	SpawnPoint({174, 14}, SpawnPoint::Mine),
	SpawnPoint({177, 17}, SpawnPoint::Mine),
	SpawnPoint({179, 13}, SpawnPoint::Mine),
	SpawnPoint({183, 11}, SpawnPoint::Mine),
	SpawnPoint({185, 15}, SpawnPoint::Mine),
	SpawnPoint({189, 17}, SpawnPoint::Mine),
	SpawnPoint({194, 18}, SpawnPoint::Mine),
	SpawnPoint({31, 138}, SpawnPoint::Mine),
	SpawnPoint({37, 143}, SpawnPoint::Mine),
	SpawnPoint({27, 143}, SpawnPoint::Mine),
	SpawnPoint({24, 135}, SpawnPoint::Mine),
	SpawnPoint({16, 130}, SpawnPoint::Mine),
	SpawnPoint({23, 128}, SpawnPoint::Mine),
	SpawnPoint({206, 84}, SpawnPoint::Bomber),
	SpawnPoint({163, 99}, SpawnPoint::Bomber),
	SpawnPoint({91, 121}, SpawnPoint::Bomber),
	SpawnPoint({149, 111}, SpawnPoint::Bomber),
	SpawnPoint({102, 95}, SpawnPoint::Bomber),
	SpawnPoint({173, 164}, SpawnPoint::Turret),
	SpawnPoint({207, 147}, SpawnPoint::Turret),
	SpawnPoint({67, 93}, SpawnPoint::Tank),
	SpawnPoint({53, 101}, SpawnPoint::Tank),
	SpawnPoint({57, 114}, SpawnPoint::Tank),
	SpawnPoint({66, 120}, SpawnPoint::Tank),
	SpawnPoint({75, 106}, SpawnPoint::Tank),
	SpawnPoint({25, 51}, SpawnPoint::Mech),
	SpawnPoint({66, 110}, SpawnPoint::Mech),
	SpawnPoint({70, 115}, SpawnPoint::Mech),
	SpawnPoint({57, 119}, SpawnPoint::Mech),
	SpawnPoint({71, 104}, SpawnPoint::Mech),
	SpawnPoint({81, 97}, SpawnPoint::Mech),
	SpawnPoint({117, 92}, SpawnPoint::Mech),
	SpawnPoint({130, 97}, SpawnPoint::Mech),
	SpawnPoint({126, 102}, SpawnPoint::Mech),
	SpawnPoint({117, 106}, SpawnPoint::Mech),
	SpawnPoint({103, 119}, SpawnPoint::Mech),
	SpawnPoint({111, 113}, SpawnPoint::Mech),
	SpawnPoint({148, 125}, SpawnPoint::Mech),
	SpawnPoint({133, 130}, SpawnPoint::Mech),
	SpawnPoint({159, 138}, SpawnPoint::Mech),
	SpawnPoint({167, 135}, SpawnPoint::Mech),
	SpawnPoint({147, 149}, SpawnPoint::Mech),
	SpawnPoint({168, 156}, SpawnPoint::Mech),
	SpawnPoint({195, 96}, SpawnPoint::Mech),
	SpawnPoint({200, 95}, SpawnPoint::Mech),
	SpawnPoint({198, 97}, SpawnPoint::Mech),
	SpawnPoint({196, 72}, SpawnPoint::Mech),
	SpawnPoint({174, 84}, SpawnPoint::Mech),
	SpawnPoint({193, 76}, SpawnPoint::Mech),
	SpawnPoint({172, 54}, SpawnPoint::Mech),
	SpawnPoint({176, 63}, SpawnPoint::Mech),
	SpawnPoint({108, 118}, SpawnPoint::Tank),
	SpawnPoint({142, 126}, SpawnPoint::Tank),
	SpawnPoint({154, 123}, SpawnPoint::Turret),
	SpawnPoint({161, 122}, SpawnPoint::Tank),
	SpawnPoint({146, 99}, SpawnPoint::Mech),
	SpawnPoint({155, 91}, SpawnPoint::Mech),
	SpawnPoint({178, 98}, SpawnPoint::Mech),
	SpawnPoint({179, 68}, SpawnPoint::Tank),
	SpawnPoint({173, 64}, SpawnPoint::Tank),
	SpawnPoint({147, 48}, SpawnPoint::Tank),
	SpawnPoint({62, 64}, SpawnPoint::Tank),
	SpawnPoint({73, 78}, SpawnPoint::Tank),
	SpawnPoint({67, 79}, SpawnPoint::Tank),
	SpawnPoint({77, 72}, SpawnPoint::Helicopter),
	SpawnPoint({132, 67}, SpawnPoint::Helicopter),
	SpawnPoint({192, 25}, SpawnPoint::Helicopter),
	SpawnPoint({99, 15}, SpawnPoint::Helicopter),
	SpawnPoint({43, 131}, SpawnPoint::Helicopter),
	SpawnPoint({92, 178}, SpawnPoint::Tank),
	SpawnPoint({117, 201}, SpawnPoint::Tank),
	SpawnPoint({128, 196}, SpawnPoint::Mech),
	SpawnPoint({128, 202}, SpawnPoint::Mech),
	SpawnPoint({137, 201}, SpawnPoint::Mech),
	SpawnPoint({105, 191}, SpawnPoint::Mech),
	SpawnPoint({115, 181}, SpawnPoint::Mech),
	SpawnPoint({74, 181}, SpawnPoint::Mech),
	SpawnPoint({82, 171}, SpawnPoint::Mech),
	SpawnPoint({64, 166}, SpawnPoint::Mech),
	SpawnPoint({53, 174}, SpawnPoint::Mech),
	SpawnPoint({31, 163}, SpawnPoint::Mech),
	SpawnPoint({30, 153}, SpawnPoint::Mech),
	SpawnPoint({33, 157}, SpawnPoint::Mech),
	SpawnPoint({37, 153}, SpawnPoint::Mech),
	SpawnPoint({20, 134}, SpawnPoint::Mech),
	SpawnPoint({17, 119}, SpawnPoint::Mech),
	SpawnPoint({120, 10}, SpawnPoint::Mech),
	SpawnPoint({126, 10}, SpawnPoint::Mech),
	SpawnPoint({123, 16}, SpawnPoint::Mech),
	SpawnPoint({108, 16}, SpawnPoint::Mech),
	SpawnPoint({106, 30}, SpawnPoint::Tank),
	SpawnPoint({126, 27}, SpawnPoint::Tank),
	SpawnPoint({172, 34}, SpawnPoint::Tank),
	SpawnPoint({204, 23}, SpawnPoint::Tank),
	SpawnPoint({206, 14}, SpawnPoint::Mech),
	SpawnPoint({209, 15}, SpawnPoint::Mech),
	SpawnPoint({184, 132}, SpawnPoint::Tank),
	SpawnPoint({187, 145}, SpawnPoint::Tank),
	SpawnPoint({188, 125}, SpawnPoint::Tank),
	SpawnPoint({119, 124}, SpawnPoint::Bomber),
	SpawnPoint({15, 66}, SpawnPoint::Mech),
	SpawnPoint({28, 63}, SpawnPoint::Mech),
	SpawnPoint({30, 2}, SpawnPoint::Blueprint),
	SpawnPoint({30, 3}, SpawnPoint::Mech),
	SpawnPoint({26, 99}, SpawnPoint::HackingKit),
	SpawnPoint({35, 118}, SpawnPoint::KeyB),
	SpawnPoint({37, 43}, SpawnPoint::Blueprint),
	SpawnPoint({55, 48}, SpawnPoint::Mech),
	SpawnPoint({71, 53}, SpawnPoint::Mech),
	SpawnPoint({67, 50}, SpawnPoint::Mech),
	SpawnPoint({96, 48}, SpawnPoint::Mine),
	SpawnPoint({101, 52}, SpawnPoint::Mine),
	SpawnPoint({104, 55}, SpawnPoint::Mine),
	SpawnPoint({106, 50}, SpawnPoint::Mine),
	SpawnPoint({159, 69}, SpawnPoint::Blueprint),
	SpawnPoint({174, 81}, SpawnPoint::Mech),
	SpawnPoint({211, 112}, SpawnPoint::Blueprint),
	SpawnPoint({54, 129}, SpawnPoint::Mech),
	SpawnPoint({62, 129}, SpawnPoint::Mech),
	SpawnPoint({55, 136}, SpawnPoint::Tank),
	SpawnPoint({64, 148}, SpawnPoint::Tank),
	SpawnPoint({67, 157}, SpawnPoint::Tank),
	SpawnPoint({34, 145}, SpawnPoint::Mech),
	SpawnPoint({111, 7}, SpawnPoint::Mech),
	SpawnPoint({137, 12}, SpawnPoint::Mech),
	SpawnPoint({133, 7}, SpawnPoint::Mech),
	SpawnPoint({124, 23}, SpawnPoint::Turret),
	SpawnPoint({124, 34}, SpawnPoint::Turret),
	SpawnPoint({103, 28}, SpawnPoint::Turret),
	SpawnPoint({154, 25}, SpawnPoint::Turret),
	SpawnPoint({161, 33}, SpawnPoint::Turret),
	SpawnPoint({172, 26}, SpawnPoint::Turret),
	SpawnPoint({127, 165}, SpawnPoint::Turret),
	SpawnPoint({137, 165}, SpawnPoint::Turret),
	SpawnPoint({137, 175}, SpawnPoint::Turret),
	SpawnPoint({127, 175}, SpawnPoint::Turret),
	SpawnPoint({76, 130}, SpawnPoint::HackingKit),
	SpawnPoint({189, 53}, SpawnPoint::HackingKit),
	SpawnPoint({73, 85}, SpawnPoint::HackingKit),
	SpawnPoint({33, 169}, SpawnPoint::HackingKit),
	SpawnPoint({182, 103}, SpawnPoint::HackingKit),
	SpawnPoint({133, 31}, SpawnPoint::HackingKit),
	SpawnPoint({45, 61}, SpawnPoint::HackingKit),
	SpawnPoint({101, 47}, SpawnPoint::HackingKit),
	SpawnPoint({141, 114}, SpawnPoint::HackingKit),
	SpawnPoint({207, 85}, SpawnPoint::Mech),
	SpawnPoint({208, 84}, SpawnPoint::Mech),
	SpawnPoint({163, 118}, SpawnPoint::Blueprint),
	SpawnPoint({65, 74}, SpawnPoint::Blueprint),
	SpawnPoint({8, 37}, SpawnPoint::Blueprint),
	SpawnPoint({154, 81}, SpawnPoint::Blueprint),
	SpawnPoint({88, 26}, SpawnPoint::Blueprint),
	SpawnPoint({170, 91}, SpawnPoint::Blueprint),
	SpawnPoint({81, 32}, SpawnPoint::Blueprint),
	SpawnPoint({149, 26}, SpawnPoint::Mech),
	SpawnPoint({157, 34}, SpawnPoint::Mech),
	SpawnPoint({168, 32}, SpawnPoint::Mech),
	SpawnPoint({182, 27}, SpawnPoint::Mech),
	SpawnPoint({182, 30}, SpawnPoint::Mech),
	SpawnPoint({185, 57}, SpawnPoint::Mech),
	SpawnPoint({177, 53}, SpawnPoint::Bomber),
	SpawnPoint({184, 63}, SpawnPoint::Bomber),
	SpawnPoint({174, 55}, SpawnPoint::Mech),
	SpawnPoint({173, 52}, SpawnPoint::Mech),
	SpawnPoint({85, 59}, SpawnPoint::Mine),
	SpawnPoint({87, 62}, SpawnPoint::Mine),
	SpawnPoint({87, 66}, SpawnPoint::Mine),
	SpawnPoint({89, 70}, SpawnPoint::Mine),
	SpawnPoint({95, 69}, SpawnPoint::Mine),
	SpawnPoint({143, 78}, SpawnPoint::Bomber),
	SpawnPoint({154, 73}, SpawnPoint::Bomber),
	SpawnPoint({170, 71}, SpawnPoint::Bomber),
	SpawnPoint({178, 125}, SpawnPoint::Bomber),
	SpawnPoint({155, 10}, SpawnPoint::Bomber),
	SpawnPoint({181, 17}, SpawnPoint::Bomber),
	SpawnPoint({192, 30}, SpawnPoint::Mech),
	SpawnPoint({202, 14}, SpawnPoint::Bomber),
	SpawnPoint({209, 22}, SpawnPoint::Bomber),
	SpawnPoint({212, 22}, SpawnPoint::Tank),
	SpawnPoint({140, 15}, SpawnPoint::Mech),
	SpawnPoint({138, 4}, SpawnPoint::Mech),
	SpawnPoint({151, 71}, SpawnPoint::Mech),
	SpawnPoint({141, 129}, SpawnPoint::Blueprint),
	SpawnPoint({230, 206}, SpawnPoint::Turret),
	SpawnPoint({224, 213}, SpawnPoint::Turret),
	SpawnPoint({224, 223}, SpawnPoint::KeyA),
	SpawnPoint({240, 217}, SpawnPoint::Mine),
	SpawnPoint({242, 216}, SpawnPoint::Mine),
	SpawnPoint({240, 213}, SpawnPoint::Mine),
	SpawnPoint({241, 207}, SpawnPoint::Mine),
	SpawnPoint({235, 206}, SpawnPoint::Mine),
	SpawnPoint({238, 203}, SpawnPoint::Mine),
	SpawnPoint({224, 202}, SpawnPoint::HackingKit),
	SpawnPoint({-28, 45}, SpawnPoint::Turret),
	SpawnPoint({-35, 72}, SpawnPoint::Turret),
	SpawnPoint({-21, 72}, SpawnPoint::Turret),
	SpawnPoint({-28, 76}, SpawnPoint::Turret),
	SpawnPoint({-21, 74}, SpawnPoint::KeyC),
	SpawnPoint({-13, 42}, SpawnPoint::KeyB),
	SpawnPoint({-41, 61}, SpawnPoint::Boat),
	SpawnPoint({-15, 51}, SpawnPoint::Boat),
	SpawnPoint({-11, 51}, SpawnPoint::Boat),
	SpawnPoint({-24, 47}, SpawnPoint::HackingKit),
	SpawnPoint({-9, 184}, SpawnPoint::Turret),
	SpawnPoint({-5, 184}, SpawnPoint::Turret),
	SpawnPoint({-11, 210}, SpawnPoint::Helicopter),
	SpawnPoint({-7, 210}, SpawnPoint::Helicopter),
	SpawnPoint({-12, 214}, SpawnPoint::Mech),
	SpawnPoint({-6, 214}, SpawnPoint::Mech),
	SpawnPoint({-13, 220}, SpawnPoint::Mech),
	SpawnPoint({-21, 216}, SpawnPoint::Mech),
	SpawnPoint({-12, 206}, SpawnPoint::Tank),
	SpawnPoint({-6, 206}, SpawnPoint::Tank),
	SpawnPoint({0, 194}, SpawnPoint::Tank),
	SpawnPoint({6, 208}, SpawnPoint::Tank),
	SpawnPoint({-36, 220}, SpawnPoint::Tank),
	SpawnPoint({-5, 193}, SpawnPoint::Mech),
	SpawnPoint({-11, 196}, SpawnPoint::Mech),
	SpawnPoint({-7, 187}, SpawnPoint::Mech),
	SpawnPoint({-16, 189}, SpawnPoint::Mech),
	SpawnPoint({-16, 200}, SpawnPoint::Mech),
	SpawnPoint({5, 217}, SpawnPoint::Mech),
	SpawnPoint({8, 217}, SpawnPoint::Mech),
	SpawnPoint({-43, 213}, SpawnPoint::Mech),
	SpawnPoint({-21, 204}, SpawnPoint::Turret),
	SpawnPoint({-32, 188}, SpawnPoint::Boat),
	SpawnPoint({-24, 188}, SpawnPoint::Boat),
	SpawnPoint({-36, 208}, SpawnPoint::Boat),
	SpawnPoint({-6, 203}, SpawnPoint::Blueprint),
	SpawnPoint({6, 206}, SpawnPoint::Mine),
	SpawnPoint({7, 198}, SpawnPoint::Mine),
	SpawnPoint({10, 200}, SpawnPoint::Mine),
	SpawnPoint({13, 197}, SpawnPoint::Mine),
	SpawnPoint({11, 186}, SpawnPoint::Mine),
	SpawnPoint({8, 183}, SpawnPoint::Mine),
	SpawnPoint({13, 184}, SpawnPoint::Mine),
	SpawnPoint({10, 181}, SpawnPoint::Mine),
	SpawnPoint({-29, 214}, SpawnPoint::Mine),
	SpawnPoint({-31, 220}, SpawnPoint::Mine),
};
const int point_count = sizeof(points)/sizeof(SpawnPoint);

