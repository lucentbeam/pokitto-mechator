#include "pickups.h"

#include "game/enums.h"
#include "game/constants.h"
#include "game/variables.h"
#include "game/ui/ui.h"
#include "core/audiosystem.h"
#include "game/states/collectblueprintprompt.h"
#include "game/maps/worldmutables.h"

ObjectPool<Pickups, 10> Pickups::s_temporary;
ObjectPool<Pickups, 6> Pickups::s_special;

std::vector<uint16_t> Pickups::s_acquired_specials;

void Pickups::configure(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&), uint16_t lifetime)
{
    position = pos;
    m_sprite = SpriteWrapper(spr);
    m_on_collect = on_collect;
    m_lifetime = lifetime;
}

void Pickups::spawnTemporary(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&), uint16_t lifetime)
{
    s_temporary.activateNext()->configure(pos, spr, on_collect, lifetime);
}

void Pickups::spawnSpecial(const Vec2i &pos, SpriteName spr, void (*on_collect)(const Vec2i&))
{
    s_special.activateNext()->configure(pos + Vec2i(3, 3), spr, on_collect, std::numeric_limits<uint16_t>::max());
}

bool Pickups::mapIndexUnacquired(const Vec2i &pos)
{
    return std::find(s_acquired_specials.begin(), s_acquired_specials.end(), MapManager::getMapIndex(pos.x(), pos.y())) == s_acquired_specials.end();
}

void Pickups::acquireAtIndex(const Vec2i &pos)
{
    s_acquired_specials.push_back(MapManager::getMapIndex(pos.x(), pos.y()));
}

void Pickups::spawnDollar(const Vec2f &pos)
{
    spawnTemporary({pos.x(), pos.y()}, DollarSprite, [](const Vec2i &pos) {
        GameVariables::changeDollars(1);
        AudioSystem::play(sfxGetDollar);
        UI::showForDuration(UI::Element::UIDollarCount, 2.0f);
    }, 600);
}

void Pickups::spawnHackingKit(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, HackingKitSprite, [](const Vec2i &pos) {
            GameVariables::changeHackingKits(1);
            acquireAtIndex(pos);
            AudioSystem::play(sfxGetItem);
            UI::showForDuration(UI::Element::UIHackingKitCount, 2.0f);
        });
    }
}

void Pickups::spawnKeycardA(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard1Sprite, [](const Vec2i &pos) {
            GameVariables::changeKeysA(1);
            acquireAtIndex(pos);
            AudioSystem::play(sfxGetItem);
            UI::showForDuration(UI::Element::UIKeyACount, 2.0f);
        });
    }
}

void Pickups::spawnKeycardB(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard2Sprite, [](const Vec2i &pos) {
            GameVariables::changeKeysB(1);
            acquireAtIndex(pos);
            AudioSystem::play(sfxGetItem);
            UI::showForDuration(UI::Element::UIKeyBCount, 2.0f);
        });
    }
}

void Pickups::spawnKeycardC(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, Keycard3Sprite, [](const Vec2i &pos) {
            GameVariables::changeKeysC(1);
            acquireAtIndex(pos);
            AudioSystem::play(sfxGetItem);
            UI::showForDuration(UI::Element::UIKeyCCount, 2.0f);
        });
    }
}

int fetchBlueprintIndex(const Vec2i &pos) {
    const int bpcount = Blueprints::LastIdxBP;
    int pidx = MapManager::getMapIndex(pos.x(), pos.y());
    int idx = 0;
    while(pidx != *(blueprints_data + idx) && idx < bpcount) {
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
        if (Player::canGetPickups() && (Player::position() - current->position).length() < 6) {
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
        if (Player::canGetPickups() && (Player::position() - current->position).length() < 6) {
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
    while (i >= 0) {
        Vec2f p = Camera::worldToScreen(start[i].position);
        RenderSystem::sprite(p.x() - 4, p.y() - 4, start[i].m_sprite.data(), start[i].m_sprite.data()[2]);

        --i;
    }
    i = s_special.objectCount()-1;
    start = s_special.objects();
    while (i >= 0) {
        Vec2f p = Camera::worldToScreen(start[i].position);
        RenderSystem::sprite(p.x() - 4, p.y() - 4, start[i].m_sprite.data(), start[i].m_sprite.data()[2]);

        --i;
    }
}
