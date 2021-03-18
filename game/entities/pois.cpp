#include "pois.h"

#include "game/states/opendoorprompt.h"
#include "game/states/openshopprompt.h"
#include "game/states/shop.h"

ObjectPool<POIs, 6> POIs::s_pois;
std::vector<uint16_t> POIs::s_activated;
POIs * POIs::s_current_active_poi = nullptr;

void POIs::configure(const Vec2f &pos, std::initializer_list<const uint8_t *> spriteFrames, float spriteFPS)
{
    m_position = pos;
    m_sprite = SpriteWrapper(spriteFrames, spriteFPS);
}

bool POIs::mapIndexUnopened(const Vec2f &pos)
{
    return std::find(s_activated.begin(), s_activated.end(), MapManager::getMapIndex(pos.x(), pos.y())) == s_activated.end();
}

void POIs::openAtIndex(const Vec2f &pos)
{
    s_activated.push_back(MapManager::getMapIndex(pos.x(), pos.y()));
}

void POIs::spawnDoor(const Vec2f &pos, uint16_t left, uint16_t top, uint8_t width, uint8_t height, POIType door)
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
    p->m_sprite = SpriteWrapper({poi[1+uint8_t(door)]}, 8.0f);
}

void POIs::spawnShop(const Vec2f &pos, const Vec2f &jeep_loc, const Vec2f &boat_loc, const Vec2f &heli_loc)
{
    POIs * p = s_pois.activateNext();
    if (p == nullptr) {
        return;
    }
    p->m_position = pos;
    p->m_jeep_loc = jeep_loc;
    p->m_boat_loc = boat_loc;
    p->m_heli_loc = heli_loc;
    p->m_door_type = POIType::Shop;
    int idx = mapIndexUnopened(pos) ? 0 : 1;
    p->m_sprite = SpriteWrapper({poi[idx]}, 8.0f);
}

void POIs::unlockCurrent()
{
    if (s_current_active_poi == nullptr) {
        return;
    }
    if (mapIndexUnopened(s_current_active_poi->m_position)) {
        openAtIndex(s_current_active_poi->m_position);
        if (s_current_active_poi->m_door_type == Shop) {
            showShop();
            s_current_active_poi->m_sprite = SpriteWrapper({poi[1]}, 8.0f);
        } else {
            for(int x = s_current_active_poi->m_left; x < (s_current_active_poi->m_width + s_current_active_poi->m_left); x+=6) {
                for(int y = s_current_active_poi->m_top; y < (s_current_active_poi->m_height + s_current_active_poi->m_top); y+=6) {
                    MapManager::setTileAt(x, y, SpecialTiles::BaseGround);
                }
            }
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
        if ((p->m_position - c).length() < 3) {
            any_in_range = true;
            if (s_current_active_poi != p) {
                s_current_active_poi = p;
                if (p->m_door_type == Shop) {
                    if (mapIndexUnopened(p->m_position)) {
                        showOpenShopPrompt();
                    } else {
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
    case PlayerMode::SoldierMode:
        return s_current_active_poi->m_position + Vec2f(3, 3);
        break;
    case PlayerMode::JeepMode:
        return s_current_active_poi->m_jeep_loc + Vec2f(3, 3);
        break;
    case PlayerMode::TankMode:
        return s_current_active_poi->m_jeep_loc + Vec2f(3, 3);
        break;
    case PlayerMode::BoatMode:
        return s_current_active_poi->m_boat_loc + Vec2f(3, 3);
        break;
    case PlayerMode::HelicopterMode:
        return s_current_active_poi->m_heli_loc + Vec2f(3, 3);
        break;
    }
}
