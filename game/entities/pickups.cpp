#include "pickups.h"

#include "game/enums.h"
#include "game/constants.h"
#include "game/variables.h"
#include "core/audiosystem.h"
#include "game/states/collectblueprintprompt.h"
#include "game/maps/worldtiles.h"
//#include "game/maps/worldmutables.h"

ObjectPool<Pickups, 14> Pickups::s_temporary;
ObjectPool<Pickups, 10> Pickups::s_special;

void Pickups::configure(const Vec2i &pos, SpriteName spr, uint16_t lifetime)
{
    position = pos;
    m_sprite = SpriteWrapper(spr);
    m_lifetime = lifetime;
}

void Pickups::spawnTemporary(const Vec2i &pos, SpriteName spr, uint16_t lifetime)
{
    Pickups * p = s_temporary.activateNext();
    if (p != nullptr) {
        p->configure(pos, spr, lifetime);
    }
}

void Pickups::spawnSpecial(const Vec2i &pos, SpriteName spr)
{
    s_special.activateNext()->configure(pos, spr, std::numeric_limits<uint16_t>::max());
}

bool Pickups::mapIndexUnacquired(const Vec2i &pos)
{
    int idx = MapManager::getMapIndex(pos.x(), pos.y());
    for (int i = 0; i < specials_count; ++i) {
        if (GameVariables::acquiredSpecials()[i] == idx) {
            return false;
        } else if (GameVariables::acquiredSpecials()[i] == 0) {
            return true;
        }
    }
    return true;
}

void Pickups::acquireAtIndex(const Vec2i &pos, void(*func)(int8_t), UI::Element id)
{
    int idx = MapManager::getMapIndex(pos.x(), pos.y());
    for (int i = 0; i < specials_count; ++i) {
        if (GameVariables::acquiredSpecials()[i] == 0) {
            GameVariables::acquiredSpecials()[i] = idx;
            AudioSystem::play(sfxGetItem);
            func(1);
            UI::showForDuration(id, 2.0f);
            return;
        }
    }
}

void Pickups::spawnDollar(const Vec2f &pos)
{
    spawnTemporary({pos.x(), pos.y()}, DollarSprite, 480);
}

void Pickups::spawnCashBox(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) spawnSpecial(pos, CashboxSprite);
}

void Pickups::spawnHackingKit(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) spawnSpecial(pos, HackingKitSprite);
}

void Pickups::spawnKeycardA(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard1Sprite);
    }
}

void Pickups::spawnKeycardB(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard2Sprite);
    }
}

void Pickups::spawnKeycardC(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard3Sprite);
    }
}

int fetchBlueprintIndex(const Vec2i &pos) {
    const int bpcount = Blueprints::LastIdxBP;
    int pidx = MapManager::getMapIndex(pos.x(), pos.y());
    int idx = 0;
    while(pidx != *(blueprints_indices + idx) && idx < bpcount) {
        idx++;
    }
    if (idx >= bpcount) return -1;
    return idx;
}

void Pickups::spawnBlueprint(const Vec2i &pos)
{
    int idx = fetchBlueprintIndex(pos);
    if (idx == -1) return;
    if (!GameVariables::hasBlueprint(idx)) {
        spawnSpecial(pos, BlueprintSprite);
    }
}

void Pickups::update(float dt)
{
    int i = s_temporary.objectCount()-1;
    Pickups * start = s_temporary.objects();
    while (i >= 0) {
        Pickups * current = start + i;
        if (Player::canGetPickups() && (Player::position() - current->position).length() < Player::pickupDistance()) {
            GameVariables::changeDollars(1);
            AudioSystem::play(sfxGetDollar);
            UI::showForDuration(UI::Element::UIDollarCount, 2.0f);
            s_temporary.deactivate(i);
            --i;
            continue;
        }

        --current->m_lifetime;
        if (current->m_lifetime == 0 || !Camera::inActiveZone(current->position, Vec2f(4, 4))) {
            s_temporary.deactivate(i);
        }

        --i;
    }

    i = s_special.objectCount()-1;
    start = s_special.objects();
    while (i >= 0) {
        Pickups * current = start + i;
        if (!Camera::inActiveZone(start[i].position)) {
            s_special.deactivate(i);
            --i;
            continue;
        }
        if (Player::canGetPickups() && (Player::position() - current->position - Vec2f(3, 3)).length() < Player::pickupDistance()) {
            switch(current->m_sprite.getSpriteName()) {
            case Keycard1Sprite:
                acquireAtIndex(current->position, GameVariables::changeKeysA, UI::UIKeyACount);
                break;
            case Keycard2Sprite:
                acquireAtIndex(current->position, GameVariables::changeKeysB, UI::UIKeyBCount);
                break;
            case Keycard3Sprite:
                acquireAtIndex(current->position, GameVariables::changeKeysC, UI::UIKeyCCount);
                break;
            case HackingKitSprite:
                acquireAtIndex(current->position, GameVariables::changeHackingKits, UI::UIHackingKitCount);
                break;
            case BlueprintSprite:
                GameVariables::acquireBlueprint(fetchBlueprintIndex(current->position));
                showBlueprint(Blueprints(fetchBlueprintIndex(current->position)));
                break;
            case CashboxSprite:
                acquireAtIndex(current->position, GameVariables::changeDollarsBig, UI::UIDollarCount);
                break;
            default:
                break;
            }

            s_special.deactivate(i);
            --i;
            continue;
        }

        --current->m_lifetime;
        if (current->m_lifetime == 0) {
            s_special.deactivate(i);
        }

        --i;
    }
}

void Pickups::draw()
{
    int i = s_temporary.objectCount()-1;
    Pickups * start = s_temporary.objects();

    uint16_t mask = Helpers::getMask({Terrain::Wall});
    while (i >= 0) {
        if (CollisionManager::collides(start[i].position, mask)) { --i; continue; }
        Vec2f p = Camera::worldToScreen(start[i].position);
//        RenderSystem::drawRect(p.x() - Player::pickupDistance(), p.y() - Player::pickupDistance(), Player::pickupDistance() * 2, Player::pickupDistance() * 2, 14);
        RenderSystem::sprite(p.x() - 4, p.y() - 4, start[i].m_sprite.data(), start[i].m_sprite.data()[2]);
        --i;
    }
    i = s_special.objectCount()-1;
    start = s_special.objects();
    while (i >= 0) {
        if (CollisionManager::collides(start[i].position, mask)) { --i; continue; }
        Vec2f p = Camera::worldToScreen(start[i].position);
//        RenderSystem::drawRect(p.x() - Player::pickupDistance() + 3, p.y() - Player::pickupDistance() + 3, Player::pickupDistance() * 2, Player::pickupDistance() * 2, 14);
        RenderSystem::sprite(p.x() - 1, p.y() - 1, start[i].m_sprite.data(), start[i].m_sprite.data()[2]);
        --i;
    }
}

void Pickups::clear()
{
    s_temporary = ObjectPool<Pickups, 14>();
    s_special = ObjectPool<Pickups, 10>();
}
