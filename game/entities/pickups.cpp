#include "pickups.h"

void Pickups::configure(const Vec2f &pos, std::initializer_list<const uint8_t *> spriteFrames, float spriteFPS, void (*on_collect)(), uint16_t lifetime)
{
    position = pos;
    m_sprite = SpriteWrapper(spriteFrames, spriteFPS);
    m_on_collect = on_collect;
    m_lifetime = lifetime;
}

void Pickups::spawnTemporary(const Vec2f &pos, std::initializer_list<const uint8_t *> spriteFrames, float spriteFPS, void (*on_collect)(), uint16_t lifetime)
{
    s_temporary.activateNext()->configure(pos, spriteFrames, spriteFPS, on_collect, lifetime);
}

void Pickups::spawnSpecial(const Vec2f &pos, std::initializer_list<const uint8_t *> spriteFrames, float spriteFPS, void (*on_collect)())
{
    s_special.activateNext()->configure(pos, spriteFrames, spriteFPS, on_collect, std::numeric_limits<uint16_t>::max());
}

void Pickups::spawnDollar(const Vec2f &pos)
{
    spawnTemporary(pos, {pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[0],pickup_dollar[1]}, 8.0f, []() {

    }, 600);
}

void Pickups::update(float dt)
{
    int i = s_temporary.objectCount()-1;
    Pickups * start = s_temporary.objects();
    while (i >= 0) {
        Pickups * current = start + i;
        current->m_sprite.update();
//        if (!Camera::inActiveZone(start[i].position)) {
//            s_temporary.deactivate(i);
//            --i;
//            continue;
//        }
        if ((Player::position() - current->position).length() < 6) {
            current->m_on_collect();
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
}

void Pickups::draw(RenderSystem *renderer)
{
    int i = s_temporary.objectCount()-1;
    Pickups * start = s_temporary.objects();
    while (i >= 0) {
        Vec2f p = Camera::worldToScreen(start[i].position);
        renderer->sprite(p.x() - 4, p.y() - 4, start[i].m_sprite.data(), start[i].m_sprite.data()[2]);

        --i;
    }
}
