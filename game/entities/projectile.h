#ifndef BULLET_H
#define BULLET_H

#include <cstdint>

#include "game/physics/body.h"
#include "core/rendersystem.h"
#include "core/rendering/spritewrapper.h"
#include "core/utilities/rect.h"

#include "core/utilities/objectpool.h"

const int maxProjectileCount = 24;

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
    SpriteWrapper sprite;
    Bounds m_bounds;
    float m_lifetime, z, vz;
    void (*m_on_expire)(Projectile*);

    Vec2f target, direction;

    uint16_t mask;
    uint8_t damage;
    bool ignore_walls = false;
    bool destroy_on_ground = false;
    bool flipped = false;
    bool is_missile = false;
    uint8_t counter = 0;

    friend ProjectileManager;
public:
    Projectile();

    void configure(const Vec2f &pos, const Vec2f &vel, int w, int h, float lifetime);

    Projectile * setExpireCallback(void (*expire_callback)(Projectile*));

    Projectile * setSprite(SpriteName spr);

    Projectile * clearSprite();

    Projectile * setTargetMask(std::initializer_list<uint8_t> mask_enums);

    Projectile * setDamage(int d) { damage = d; return this; }

    Projectile * setIgnoreWalls() { ignore_walls = true; return this; }

    Projectile * setInAir(float pz, float pvz, bool destroy_at_ground = true);

    Projectile * addVelocity(const Vec2f &vel);

    Projectile * setFlipped(bool flip) { flipped = flip; return this; }

    Projectile * setMissile(const Vec2f &t, const Vec2f &d) { target = t; direction = d; is_missile = true; counter = rand() % 5; return this; }

    void update(float dt);

    void draw();

    void drawAir();

    Vec2f pos() const { return m_body.pos(); }

    bool expired() const { return m_lifetime <= 0; }

    void onExpire();
};

class ProjectileManager
{
    static ObjectPool<Projectile, maxProjectileCount> s_projectiles;

    static Projectile s_dummy;

public:

    static Projectile* create(const Vec2f &pos, const Vec2f &vel, int size, float lifetime, int size_w = -1);

    static void update(float dt);

    static void draw();

    static void drawAir();

    static int getCollisionDamage(const Vec2f &pos, int size, uint16_t mask);

    static int getCollisionDamage(const Rect &rect, uint16_t mask);

    static int getCollisionDamage(const Rect &rect, uint16_t mask, std::vector<Vec2f> &at);
};

#endif // BULLET_H
