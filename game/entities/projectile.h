#ifndef BULLET_H
#define BULLET_H

#include <cstdint>

#include "game/physics/body.h"
#include "core/rendering/rendersystem.h"

class Projectile
{
    Body m_body;
    float m_lifetime;
    void (*m_hit_wall)(Projectile*,uint16_t);
    void (*m_hit_entity)(Projectile*,uint16_t,void*);

public:
    Projectile();

    Projectile(const Vec2f &pos, const Vec2f &vel, float lifetime);

    void setWallCollide(void (*wall_collide_callback)(Projectile*,uint16_t));

    void setEntityCollide(void (*enemy_collide_callback)(Projectile*,uint16_t,void*));

    void update(float dt);

    void draw(RenderSystem * renderer);

    Vec2f pos() const { return m_body.pos(); }

    bool expired() const { return m_lifetime <= 0; }
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
