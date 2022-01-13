#include "pickups.h"

#include "game/variables.h"
#include "game/ui/ui.h"

ObjectPool<Pickups, 10> Pickups::s_temporary;
ObjectPool<Pickups, 4> Pickups::s_special;

std::vector<uint16_t> Pickups::s_acquired_specials;

void Pickups::configure(const Vec2i &pos, const uint8_t * spriteFrames, int framecount, float spriteFPS, void (*on_collect)(const Vec2i&), uint16_t lifetime)
{
    position = pos;
    m_sprite = SpriteWrapper(spriteFrames, framecount, spriteFPS);
    m_on_collect = on_collect;
    m_lifetime = lifetime;
}

void Pickups::spawnTemporary(const Vec2i &pos,  const uint8_t * spriteFrames, int framecount, float spriteFPS, void (*on_collect)(const Vec2i&), uint16_t lifetime)
{
    s_temporary.activateNext()->configure(pos, spriteFrames, framecount, spriteFPS, on_collect, lifetime);
}

void Pickups::spawnSpecial(const Vec2i &pos,  const uint8_t * spriteFrames, int framecount, float spriteFPS, void (*on_collect)(const Vec2i&))
{
    s_special.activateNext()->configure(pos + Vec2i(3, 3), spriteFrames, framecount, spriteFPS, on_collect, std::numeric_limits<uint16_t>::max());
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
    spawnTemporary({pos.x(), pos.y()}, pickup_dollar[0], 2, 8.0f, [](const Vec2i &pos) {
        GameVariables::changeDollars(1);
        UI::showForDuration(UI::Element::UIDollarCount, 2.0f);
    }, 600);
}

void Pickups::spawnHackingKit(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, pickup_hackingkit[0], 2, 8.0f, [](const Vec2i &pos) {
            GameVariables::changeHackingKits(1);
            acquireAtIndex(pos);
            UI::showForDuration(UI::Element::UIHackingKitCount, 2.0f);
        });
    }
}

void Pickups::spawnKeycardA(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, pickup_keycard1[0], 2, 8.0f, [](const Vec2i &pos) {
            GameVariables::changeKeysA(1);
            acquireAtIndex(pos);
            UI::showForDuration(UI::Element::UIKeyACount, 2.0f);
        });
    }
}

void Pickups::spawnKeycardB(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, pickup_keycard2[0], 2, 8.0f, [](const Vec2i &pos) {
            GameVariables::changeKeysB(1);
            acquireAtIndex(pos);
            UI::showForDuration(UI::Element::UIKeyBCount, 2.0f);
        });
    }
}

void Pickups::spawnKeycardC(const Vec2i &pos)
{
    if (mapIndexUnacquired(pos)) {
        spawnSpecial(pos, pickup_keycard3[0], 2, 8.0f, [](const Vec2i &pos) {
            GameVariables::changeKeysC(1);
            acquireAtIndex(pos);
            UI::showForDuration(UI::Element::UIKeyCCount, 2.0f);
        });
    }
}

void Pickups::update(float dt)
{
    int i = s_temporary.objectCount()-1;
    Pickups * start = s_temporary.objects();
    while (i >= 0) {
        Pickups * current = start + i;
        current->m_sprite.update();
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
        current->m_sprite.update();
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
