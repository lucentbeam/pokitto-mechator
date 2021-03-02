#include "projectile.h"
#include "game/rendering/camera.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"

Projectile::Projectile() :
    m_body({0, 0}, {0, 0}),
    m_lifetime(0),
    m_on_expire(nullptr)
{

}

void Projectile::configure(const Vec2f &pos, const Vec2f &vel, int size, float lifetime)
{
    m_rect = Rect(0, 0, size, size);
    m_rect.setCenter(pos.x(), pos.y());
    m_body = Body(pos, vel);
    m_lifetime = lifetime;
    m_on_expire = nullptr;
    damage = 1;
    struck = false;
    ignore_walls = false;
}

Projectile * Projectile::setExpireCallback(void (*expire_callback)(Projectile *))
{
    m_on_expire = expire_callback;
    return this;
}

Projectile * Projectile::setSprite(std::initializer_list<const uint8_t *> frames, float fps)
{
    sprite = SpriteWrapper(frames, fps);
    return this;
}

Projectile *Projectile::clearSprite()
{
    sprite = SpriteWrapper();
    return this;
}

Projectile * Projectile::setTargetMask(std::initializer_list<uint8_t> mask_enums)
{
    mask = Helpers::getMask(mask_enums);
    return this;
}

void Projectile::update(float dt)
{
    m_body.update(dt);
    m_lifetime -= dt;
    sprite.update();
    m_rect.setCenter(m_body.pos().x(), m_body.pos().y());
}

void Projectile::draw(RenderSystem *renderer)
{
    Vec2f pos = Camera::worldToScreen(m_body.pos());
    const uint8_t * spriteData = sprite.data();
    if (spriteData != nullptr) {
        renderer->sprite(pos.x()-spriteData[0]/2, pos.y()-spriteData[1]/2, spriteData, spriteData[2]);
    }
}

void Projectile::onExpire()
{
    if (m_on_expire != nullptr) {
        m_on_expire(this);
    }
}

ObjectPool<Projectile,32> ProjectileManager::s_projectiles;

Projectile *ProjectileManager::create(const Vec2f &pos, const Vec2f &vel, int size, float lifetime)
{
    Projectile * p = s_projectiles.activateNext();
    p->configure(pos, vel, size, lifetime);
    return p;
}

void ProjectileManager::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::DestrucableWood, Terrain::DestructableMetal});
    int i = s_projectiles.objectCount() - 1;
    Projectile * start = s_projectiles.objects();
    while (i >= 0) {
        bool hitWall = !start[i].ignore_walls && CollisionManager::collision(start[i].pos(), mask, Vec2f(2,2));
        if (start[i].expired() || hitWall) {
            start[i].onExpire();
            s_projectiles.deactivate(i);
        } else {
            start[i].update(dt); // let wall hits clear out next frame to give other things time to get hit
        }
        --i;
    }
}

void ProjectileManager::draw(RenderSystem *renderer)
{
    int i = 0;
    Projectile * start = s_projectiles.objects();
    while (i < s_projectiles.objectCount()) {
        start[i].draw(renderer);
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
    while (i < s_projectiles.objectCount()) {
        if (((mask & start[i].mask) == mask) && rect.overlaps(start[i].m_rect)) {
            damage += start[i].damage;
            start[i].struck = true;
        }
        ++i;
    }
    return damage;
}
