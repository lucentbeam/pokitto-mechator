#include "projectile.h"
#include "core/rendering/camera.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"
#include "game/sprites.h"
#include "game/entities/effects.h"

Projectile::Projectile() :
    m_body({0, 0}, {0, 0}),
    m_lifetime(0),
    m_on_expire(nullptr)
{

}

void Projectile::configure(const Vec2f &pos, const Vec2f &vel, int w, int h, float lifetime)
{
    m_bounds = Bounds(w, h);
    m_body = Body(pos, vel);
    m_lifetime = lifetime;
    m_on_expire = nullptr;
    damage = 1;
    ignore_walls = false;
    z = 0;
    vz = 0;
    flipped = false;
    is_missile = false;
}

Projectile * Projectile::setExpireCallback(void (*expire_callback)(Projectile *))
{
    m_on_expire = expire_callback;
    return this;
}

Projectile * Projectile::setSprite(SpriteName spr)
{
    sprite = SpriteWrapper(spr);
    return this;
}

Projectile *Projectile::clearSprite()
{
    sprite = SpriteWrapper(SpriteName::NoSprite);
    return this;
}

Projectile * Projectile::setTargetMask(std::initializer_list<uint8_t> mask_enums)
{
    mask = Helpers::getMask(mask_enums);
    return this;
}

Projectile *Projectile::setInAir(float pz, float pvz, bool destroy_at_ground)
{
    z = pz;
    vz = pvz;
    destroy_on_ground = destroy_at_ground;
    return this;
}

Projectile *Projectile::addVelocity(const Vec2f &vel)
{
    m_body.addVel(vel);
    return this;
}

void Projectile::update(float dt)
{
    if (is_missile) {
        target += direction * dt;
        m_body.steerToward(target);
        sprite = SpriteWrapper(SpriteName(int(SpriteName::MissileSprite1) + m_body.vel().getRotationFrame(8.0f)));
        flipped = m_body.vel().x() > 0;
        counter++;
        if (counter % 10 == 0) {
            EffectManager::createSmoke(m_body.pos());
            counter = rand() % 5;
        }
    }
    m_body.update(dt);
    m_lifetime -= dt;
    if (z != 0 || vz != 0) {
        z += vz * dt;
        vz = -3000 * dt;
        if (z <= 0) {
            z = 0;
            vz = 0;
            if (destroy_on_ground) m_lifetime -= 100000.0f; // hell yeah! that's a big number. no reason anything would ever be larger, amirite?
        }
    }
}

void Projectile::draw()
{
    Vec2f pos = Camera::worldToScreen(m_body.pos());
    const uint8_t * spriteData = sprite.data();
    if (spriteData != nullptr) {
        if (int(z) > 0) {
            RenderSystem::drawShadow(pos.x()-spriteData[0]/2, pos.y()-spriteData[1]/2, spriteData, spriteData[2], flipped);
        } else {
            RenderSystem::sprite(pos.x()-spriteData[0]/2, pos.y()-spriteData[1]/2, spriteData, spriteData[2], flipped);
        }
    }
}

void Projectile::drawAir()
{
    if (z == 0) return;
    Vec2f pos = Camera::worldToScreen(m_body.pos());
    const uint8_t * spriteData = sprite.data();
    if (spriteData != nullptr) {
        RenderSystem::sprite(pos.x()-spriteData[0]/2, pos.y()-spriteData[1]/2 - z, spriteData, spriteData[2]);
    }
}

void Projectile::onExpire()
{
    if (m_on_expire != nullptr) {
        m_on_expire(this);
    }
}

ObjectPool<Projectile,maxProjectileCount> ProjectileManager::s_projectiles;

Projectile ProjectileManager::s_dummy;

Projectile *ProjectileManager::create(const Vec2f &pos, const Vec2f &vel, int size, float lifetime, int size_w)
{
    Projectile * p = s_projectiles.activateNext();
    if (p == nullptr) p = &s_dummy;
    if (size_w == -1) size_w = size;
    p->configure(pos, vel, size_w, size, lifetime);
    return p;
}

void ProjectileManager::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::DestrucableWood, Terrain::DestructableMetal});
    int i = s_projectiles.objectCount() - 1;
    Projectile * start = s_projectiles.objects();
    static int spacer = 0;
    spacer++;
    while (i >= 0) {
        bool hitWall = (i % 2) == (spacer % 2) && !start[i].ignore_walls && CollisionManager::collision(start[i].pos(), mask, Vec2f(2,2));
        if (start[i].expired() || hitWall) {
            start[i].onExpire();
            s_projectiles.deactivate(i);
        } else {
            start[i].update(dt); // let wall hits clear out next frame to give other things time to get hit
        }
        --i;
    }
}

void ProjectileManager::draw()
{
    int i = 0;
    Projectile * start = s_projectiles.objects();
    while (i < s_projectiles.objectCount()) {
        start[i].draw();
        ++i;
    }
}

void ProjectileManager::drawAir()
{
    int i = 0;
    Projectile * start = s_projectiles.objects();
    while (i < s_projectiles.objectCount()) {
        start[i].drawAir();
        ++i;
    }
}

int ProjectileManager::getCollisionDamage(const Vec2f &pos, int size, uint16_t mask)
{
    Rect rect(0, 0, size, size);
    rect.setCenter(pos.x(), pos.y());
    return getCollisionDamage(rect, mask);
}

int ProjectileManager::getCollisionDamage(const Rect &rect, uint16_t mask)
{
    int damage = 0;
    int i = 0;
    Projectile * start = s_projectiles.objects();
    Rect r;
    while (i < s_projectiles.objectCount()) {
        if ((mask & start[i].mask) != mask) {
            ++i;
            continue;
        }
        r = Rect(start[i].m_body.pos().x(), start[i].m_body.pos().y() - start[i].z, start[i].m_bounds);
        if (rect.overlaps(r)) {
            damage += start[i].damage;
            start[i].m_lifetime -= 100000.0f;
        }
        ++i;
    }
    return damage;
}

int ProjectileManager::getCollisionDamage(const Rect &rect, uint16_t mask, std::vector<Vec2f> &at)
{
    int damage = 0;
    int i = 0;
    Projectile * start = s_projectiles.objects();
    while (i < s_projectiles.objectCount()) {
        if ((mask & start[i].mask) != mask) {
            ++i;
            continue;
        }
        Rect r(start[i].m_body.pos().x(), start[i].m_body.pos().y() - start[i].z, start[i].m_bounds);
        if (rect.overlaps(r)) {
            damage += start[i].damage;
            start[i].m_lifetime -= 100000.0f;
            at.push_back(start[i].m_body.pos());
        }
        ++i;
    }
    return damage;
}
