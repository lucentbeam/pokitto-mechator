#ifndef BULLET_H
#define BULLET_H

#include <cstdint>
#include <initializer_list>

#include "game/physics/body.h"
#include "core/rendering/rendersystem.h"

class SpriteWrapper {
public:
    const uint8_t * data;
    SpriteWrapper() : data(nullptr) {}
    SpriteWrapper(const uint8_t * spr) : data(spr) {}
};

class Projectile
{
    Body m_body;
    float m_lifetime;
    void (*m_on_expire)(Projectile*);
    void (*m_hit_entity)(Projectile*,uint16_t,void*);

    SpriteWrapper sprite;
public:
    Projectile();

    Projectile(const Vec2f &pos, const Vec2f &vel, float lifetime);

    void setExpireCallback(void (*expire_callback)(Projectile*));

    void setEntityCollide(void (*enemy_collide_callback)(Projectile*,uint16_t,void*));

    void setSprite(const uint8_t * spr);

    void setSprite(std::initializer_list<const uint8_t*> frames, float fps);

    void update(float dt);

    void draw(RenderSystem * renderer);

    Vec2f pos() const { return m_body.pos(); }

    bool expired() const { return m_lifetime <= 0; }

    void onExpire();
};

class ProjectileManager
{
    static Projectile s_projectiles[32];

    static uint8_t s_projectileCount;

public:

    static Projectile* create(const Vec2f &pos, const Vec2f &vel, float lifetime);

    static void update(float dt);

    static void draw(RenderSystem * renderer);
};

#endif // BULLET_H
