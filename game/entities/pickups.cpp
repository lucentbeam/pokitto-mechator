#include "pickups.h"

#include "game/enums.h"
#include "game/constants.h"
#include "game/variables.h"
#include "core/audiosystem.h"
#include "game/states/collectblueprintprompt.h"
#include "game/maps/worldtiles.h"
//#include "game/maps/worldmutables.h"

ObjectPool<Pickups, 8> Pickups::s_temporary;
ObjectPool<Pickups, 6> Pickups::s_special;

void Pickups::configure(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&), uint16_t lifetime)
{
    position = pos;
    m_sprite = SpriteWrapper(spr);
    m_on_collect = on_collect;
    m_lifetime = lifetime;
}

void Pickups::spawnTemporary(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&), uint16_t lifetime)
{
    Pickups * p = s_temporary.activateNext();
    if (p != nullptr) {
        p->configure(pos, spr, on_collect, lifetime);
    }
}

void Pickups::spawnSpecial(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&))
{
    s_special.activateNext()->configure(pos, spr, on_collect, std::numeric_limits<uint16_t>::max());
}

bool Pickups::mapIndexUnacquired(const Vec2i &pos)
{
    int idx = MapManager::getMapIndex(pos.x(), pos.y());
    for (int i = 0; i < 100; ++i) {
        if (GameVariables::acquiredSpecials()[i] == idx) {
            return false;
        }
    }
    return true;
}

void Pickups::acquireAtIndex(const Vec2i &pos, void(*func)(int8_t), UI::Element id)
{
    int idx = MapManager::getMapIndex(pos.x(), pos.y());
    for (int i = 0; i < 100; ++i) {
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
    spawnTemporary({pos.x(), pos.y()}, DollarSprite, [](const Vec2i &pos) {
        GameVariables::changeDollars(1);
        AudioSystem::play(sfxGetDollar);
        UI::showForDuration(UI::Element::UIDollarCount, 2.0f);
    }, 480);
}

void Pickups::spawnHackingKit(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, HackingKitSprite, [](const Vec2i &pos) {
            acquireAtIndex(pos, GameVariables::changeHackingKits, UI::UIHackingKitCount);
        });
    }
}

void Pickups::spawnKeycardA(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard1Sprite, [](const Vec2i &pos) {
            acquireAtIndex(pos, GameVariables::changeKeysA, UI::Element::UIKeyACount);
        });
    }
}

void Pickups::spawnKeycardB(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard2Sprite, [](const Vec2i &pos) {
            acquireAtIndex(pos, GameVariables::changeKeysB, UI::Element::UIKeyBCount);
        });
    }
}

void Pickups::spawnKeycardC(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard3Sprite, [](const Vec2i &pos) {
            acquireAtIndex(pos, GameVariables::changeKeysC, UI::Element::UIKeyCCount);
        });
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
        spawnSpecial(pos, BlueprintSprite, [](const Vec2i &pos) {
            int idx = fetchBlueprintIndex(pos);
            GameVariables::acquireBlueprint(idx);
            showBlueprint(Blueprints(idx));
        });
    }
}

void Pickups::update(float dt)
{
    int i = s_temporary.objectCount()-1;
    Pickups * start = s_temporary.objects();
    while (i >= 0) {
        Pickups * current = start + i;
        if (Player::canGetPickups() && (Player::position() - current->position).length() < Player::pickupDistance()) {
            current->m_on_collect(current->position);
            s_temporary.deactivate(i);
            --i;
            continue;
        }

        --current->m_lifetime;
        if (current->m_lifetime == 0) {
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
            current->m_on_collect(current->position);
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
    s_temporary = ObjectPool<Pickups, 8>();
    s_special = ObjectPool<Pickups, 6>();
}
