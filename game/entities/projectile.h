#ifndef BULLET_H
#define BULLET_H

#include <cstdint>
#include <vector>

#include "game/physics/body.h"
#include "core/rendersystem.h"
#include "core/rendering/spritewrapper.h"
#include "core/utilities/rect.h"

#include "core/utilities/objectpool.h"

enum Targets {
    PlayerTarget = 1,
    EnemyTarget = 2,
    GroundTarget = 3,
    AirTarget = 4
};

class ProjectileManager;

class Projectile
{
    Body m_body;
    Rect m_rect;
    float m_lifetime, z, vz;
    void (*m_on_expire)(Projectile*);

    SpriteWrapper sprite;

    uint16_t mask;
    uint8_t damage;
    bool struck = false;
    bool ignore_walls = false;
    bool destroy_on_ground = false;

    friend ProjectileManager;
public:
    Projectile();

    void configure(const Vec2f &pos, const Vec2f &vel, int size, float lifetime);

    Projectile * setExpireCallback(void (*expire_callback)(Projectile*));

    Projectile * setSprite(std::initializer_list<const uint8_t*> frames, float fps);

    Projectile * clearSprite();

    Projectile * setTargetMask(std::initializer_list<uint8_t> mask_enums);

    Projectile * setDamage(int d) { damage = d; return this; }

    Projectile * setIgnoreWalls() { ignore_walls = true; return this; }

    Projectile * setInAir(float pz, float pvz, bool destroy_at_ground = true);

    void update(float dt);

    void draw();

    void drawAir();

    Vec2f pos() const { return m_body.pos(); }

    bool expired() const { return m_lifetime <= 0 || struck; }

    void onExpire();
};

class ProjectileManager
{
    static ObjectPool<Projectile, 32> s_projectiles;

public:

    static Projectile* create(const Vec2f &pos, const Vec2f &vel, int size, float lifetime);

    static void update(float dt);

    static void draw();

    static void drawAir();

    static int getCollisionDamage(const Vec2f &pos, int size, uint16_t mask);

    static int getCollisionDamage(const Rect &rect, uint16_t mask);

    static int getCollisionDamage(const Rect &rect, uint16_t mask, std::vector<Vec2f> &at);
};

#endif // BULLET_H
