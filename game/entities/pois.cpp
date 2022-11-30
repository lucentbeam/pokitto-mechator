#include "pois.h"

#include "game/states/opendoorprompt.h"
#include "game/states/openshopprompt.h"
#include "game/states/shop.h"
#include "core/audiosystem.h"
#include "game/maps/spawnpoint.h"
#include "game/maps/spawnpoints.h"
#include "game/variables.h"

ObjectPool<POIs, 6> POIs::s_pois;
POIs * POIs::s_current_active_poi = nullptr;
bool POIs::s_disable_shops = false;

void POIs::configure(const Vec2f &pos, SpriteName spr)
{
    m_position = pos;
    m_sprite = SpriteWrapper(spr);
}

bool POIs::mapIndexUnopened(const Vec2f &pos)
{
    int idx = MapManager::MapManager::getMapIndex(pos.x(), pos.y());
    for(int i = 0; i < point_count; ++i) {
        if (GameVariables::activatedDoors()[i]== idx) {
            return false;
        }
    }
    return true;
}

void POIs::openAtIndex(const Vec2f &pos)
{
    for(int i = 0; i < point_count; ++i) {
        if (GameVariables::activatedDoors()[i] == 0) {
            GameVariables::activatedDoors()[i] = MapManager::getMapIndex(pos.x(), pos.y());
            return;
        }
    }
}

void POIs::spawnDoor(const Vec2i &pos, int16_t left, int16_t top, uint8_t width, uint8_t height, uint8_t tile, POIType door)
{
    if (!mapIndexUnopened(pos)) {
        return;
    }
    POIs * p = s_pois.activateNext();
    if (p == nullptr) {
        return;
    }
    p->m_position = pos;
    p->m_left = left;
    p->m_top = top;
    p->m_width = width;
    p->m_height = height;
    p->m_door_type = door;
    p->m_tile = tile;
    p->m_sprite = SpriteWrapper(SpriteName(int(SpriteName::POIShopClosed) + int(door)));
}

void POIs::spawnShop(const Vec2i &pos, const Vec2f &jeep_loc, const Vec2f &boat_loc, const Vec2f &heli_loc)
{
    POIs * p = s_pois.activateNext();
    if (p == nullptr) {
        return;
    }
    p->m_position = pos;
    p->m_jeep_loc = jeep_loc * 6;
    p->m_boat_loc = boat_loc * 6;
    p->m_heli_loc = heli_loc * 6;
    p->m_door_type = POIType::Shop;
    //int idx = mapIndexUnopened(pos) ? 0 : 1;
    p->m_sprite = SpriteWrapper(POIShopClosed);
}

void POIs::unlockCurrent()
{
    if (s_current_active_poi == nullptr) {
        return;
    }
    if (mapIndexUnopened(s_current_active_poi->m_position)) {
        AudioSystem::play(sfxGetItem);
        openAtIndex(s_current_active_poi->m_position);
        if (s_current_active_poi->m_door_type == Shop) {
            showShop();
            s_current_active_poi->m_sprite = SpriteWrapper(POIShopOpen);
        } else {
            for(int x = s_current_active_poi->m_left; x < (s_current_active_poi->m_width + s_current_active_poi->m_left); x+=6) {
                for(int y = s_current_active_poi->m_top; y < (s_current_active_poi->m_height + s_current_active_poi->m_top); y+=6) {
                    MapManager::setTileAt(x, y, s_current_active_poi->m_tile);
                }
            }
            SpawnPoint::openDoorAt(s_current_active_poi->m_position);
            s_pois.deactivate(s_current_active_poi);
        }
    }
}

void POIs::update(float dt)
{
    Vec2f c = Vec2f(Camera::tl_x() + 55 - 3, Camera::tl_y() + 44 - 3);
    bool any_in_range = false;
    for(int i = s_pois.objectCount() - 1; i >= 0; --i) {
        POIs * p = s_pois.objects() + i;
        if (!Camera::inActiveZone(p->m_position)) {
            s_pois.deactivate(i);
            continue;
        }
        if (Player::canGetPickups() && (p->m_position - c).length() < Player::pickupDistance()) {
            any_in_range = true;
            if (s_current_active_poi != p) {
                s_current_active_poi = p;
                if (p->m_door_type == Shop) {
                    if (s_disable_shops) continue;
                    if (mapIndexUnopened(p->m_position)) {
                        showOpenShopPrompt();
                    } else {
                        AudioSystem::play(sfxConfirm);
                        showShop();
                    }
                } else {
                    if (p->m_door_type == DoorNone) {
                        unlockCurrent();
                    } else {
                        showOpenDoorPrompt(s_current_active_poi->m_door_type);
                    }
                }
            }
        }
    }
    if (!any_in_range) {
        s_current_active_poi = nullptr;
    }
}

void POIs::draw()
{
    for(int i = s_pois.objectCount() - 1; i >= 0; --i) {
        POIs * p = s_pois.objects() + i;
        if (p->m_door_type == Shop && s_disable_shops) continue;
        Vec2f pos = Camera::worldToScreen(p->m_position);
        RenderSystem::sprite(pos.x(), pos.y(), p->m_sprite.data());
    }
}

Vec2f POIs::pos(PlayerMode mode)
{
    if (s_current_active_poi == nullptr) {
        return Vec2f(0,0);
    }
    switch (mode) {
    default:
        return s_current_active_poi->m_position + Vec2f(3, 3);
    case PlayerMode::JeepMode:
        return s_current_active_poi->m_jeep_loc + Vec2f(3, 3);
    case PlayerMode::TankMode:
        return s_current_active_poi->m_jeep_loc + Vec2f(3, 3);
    case PlayerMode::BoatMode:
        return s_current_active_poi->m_boat_loc + Vec2f(3, 3);
    case PlayerMode::HelicopterMode:
        return s_current_active_poi->m_heli_loc + Vec2f(3, 3);
    }
}

bool POIs::canBuild(PlayerMode mode)
{
    if (s_current_active_poi == nullptr) {
        return false;
    }
    Vec2f ref;
    switch (mode) {
    default:
        ref = s_current_active_poi->m_position;
        break;
    case PlayerMode::JeepMode:
        ref = s_current_active_poi->m_jeep_loc;
        break;
    case PlayerMode::TankMode:
        ref = s_current_active_poi->m_jeep_loc;
        break;
    case PlayerMode::BoatMode:
        ref = s_current_active_poi->m_boat_loc;
        break;
    case PlayerMode::HelicopterMode:
        ref = s_current_active_poi->m_heli_loc;
        break;
    }
    ref = ref - s_current_active_poi->m_position;
    return ref.length() > 0;
}
