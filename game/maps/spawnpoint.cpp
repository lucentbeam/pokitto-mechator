#include "spawnpoint.h"

#include "core/rendering/camera.h"
#include <algorithm>

#include "game/maps/spawnpoints.h"
#include "game/variables.h"
#include "game/maps/worldtiles.h"
#include "game/entities/barracks.h"

bool SpawnPoint::s_active_points[point_count];

void SpawnPoint::setActiveRegion()
{
    if (!Camera::hasMovedRegions()) return;

    for (int i = 0; i < point_count; ++i) {
        if (Camera::inActiveZone(points[i].m_pos)) {
            if (!s_active_points[i]) {
                SpawnPoint::approach(points[i], i);
                bool door = points[i].m_type == DoorA || points[i].m_type == DoorB || points[i].m_type == DoorC || points[i].m_type == Shop;
                if (door && i < spawnpoint_data_count && GameVariables::doorStates()[i] == Hidden) {
                    GameVariables::doorStates()[i] = Discovered;
                }
            }
            s_active_points[i] = true;
        } else {
            s_active_points[i] = false;
        }
    }
}

void SpawnPoint::reset()
{
    for (int i = 0; i < point_count; ++i) s_active_points[i] = false;
}

void SpawnPoint::openDoorAt(const Vec2f &pos)
{
    for(int i = 0; i < spawnpoint_data_count; ++i) {
        if (points[i].pos().x() == int(pos.x()) && points[i].pos().y() == int(pos.y())) {
            GameVariables::doorStates()[i] = Opened;
        }
    }
}

void SpawnPoint::approach(const SpawnPoint &s, int idx)
{
    int16_t * dat;
    if (idx < spawnpoint_data_count) {
        dat = spawnpoint_data + spawnpoint_data_indices[idx];
    }
    switch(s.m_type) {
    case DoorA:
    case DoorB:
    case DoorC:
    case DoorNone:
        POIs::spawnDoor(s.m_pos, dat[0] * 6, dat[1] * 6, dat[2] * 6, dat[3] * 6, dat[4], POIType(s.m_type));
        break;
    case Shop:
        POIs::spawnShop(s.m_pos, {float(dat[0]), float(dat[1])}, {float(dat[2]), float(dat[3])}, {float(dat[4]), float(dat[5])});
        break;
    case HackingKit:
        Pickups::spawnHackingKit(s.m_pos);
        break;
    case KeyA:
        Pickups::spawnKeycardA(s.m_pos);
        break;
    case KeyB:
        Pickups::spawnKeycardB(s.m_pos);
        break;
    case KeyC:
        Pickups::spawnKeycardC(s.m_pos);
        break;
    case Blueprint:
        Pickups::spawnBlueprint(s.m_pos);
        break;
    case EnemyBarracks:
        Barracks::create(s.m_pos, dat[0], dat[1], dat[2], dat[3]);
        break;
    case Mine:
        Enemy::spawnMine(s.m_pos);
        break;
    case WaterMine:
        Enemy::spawnWaterMine(s.m_pos, dat[0], dat[1]);
        break;
    case Boat:
        Enemy::createBoat(s.m_pos);
        break;
    case Tank:
        Enemy::createTank(s.m_pos);
        break;
    case Mech:
        Enemy::createMech(s.m_pos);
        break;
    case Turret:
        Enemy::createTurret(s.m_pos);
        break;
    case TurretDisabled:
        Enemy::createTurretDisabled(s.m_pos);
        break;
    case Bomber:
        Enemy::createBomber(s.m_pos);
        break;
    case Helicopter:
        Enemy::createHelicopter(s.m_pos);
        break;
    case LasersVert:
        std::cout << idx << " " << spawnpoint_data_indices[idx] << std::endl;
//        Enemy::createLasers(s.m_pos, true, dat[0]);
        break;
    case LasersHoriz:
        std::cout << idx << " " << spawnpoint_data_indices[idx] << std::endl;
//        Enemy::createLasers(s.m_pos, false, dat[0]);
        break;
    }
}

