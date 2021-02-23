#include "projectile.h"
#include "game/rendering/camera.h"
#include "game/utilities/helpers.h"
#include "game/physics/collisionmanager.h"

Projectile::Projectile() :
    m_body({0, 0}, {0, 0}),
    m_lifetime(0),
    m_on_expire(nullptr),
    m_hit_entity(nullptr)
{

}

Projectile::Projectile(const Vec2f &pos, const Vec2f &vel, float lifetime) :
    m_body(pos, vel),
    m_lifetime(lifetime),
    m_on_expire(nullptr),
    m_hit_entity(nullptr)
{

}

void Projectile::setExpireCallback(void (*expire_callback)(Projectile *))
{
    m_on_expire = expire_callback;
}

void Projectile::setEntityCollide(void (*enemy_collide_callback)(Projectile *, uint16_t, void *))
{
    m_hit_entity = enemy_collide_callback;
}

void Projectile::setSprite(std::initializer_list<const uint8_t *> frames, float fps)
{
    sprite = SpriteWrapper(frames, fps);
}

void Projectile::update(float dt)
{
    m_body.update(dt);
    m_lifetime -= dt;
    sprite.update();
}

void Projectile::draw(RenderSystem *renderer)
{
    Vec2f pos = Camera::worldToScreen(m_body.pos());
    const uint8_t * spriteData = sprite.data();
    if (spriteData == nullptr) {
        renderer->drawRect(pos.x()-1,pos.y(),2,2,9);
        renderer->drawRect(pos.x()-1,pos.y()-1,2,2,41);
    } else {
        renderer->sprite(pos.x()-spriteData[0]/2, pos.y()-spriteData[1]/2, spriteData);
    }
}

void Projectile::onExpire()
{
    if (m_on_expire != nullptr) {
        m_on_expire(this);
    }
}

Projectile ProjectileManager::s_projectiles[32];

uint8_t ProjectileManager::s_projectileCount = 0;

Projectile *ProjectileManager::create(const Vec2f &pos, const Vec2f &vel, float lifetime)
{
    if (s_projectileCount >= 32) {
        return nullptr;
    }
    s_projectiles[s_projectileCount] = Projectile(pos, vel, lifetime);
    s_projectileCount++;
    return s_projectiles + s_projectileCount - 1;
}

void ProjectileManager::update(float dt)
{
    static uint16_t mask = Helpers::getMask({Terrain::Wall, Terrain::DestrucableWood, Terrain::DestructableMetal});
    int i = 0;
    while (i < s_projectileCount) {
        s_projectiles[i].update(dt);
        bool hitWall = CollisionManager::collision(s_projectiles[i].pos(), mask, Vec2f(2,2));
        if (s_projectiles[i].expired() || hitWall) {
            s_projectiles[i].onExpire();
            s_projectiles[i] = s_projectiles[s_projectileCount-1];
            s_projectileCount--;
        } else {
            i++;
        }
    }
}

void ProjectileManager::draw(RenderSystem *renderer)
{
    for (int i = 0; i < s_projectileCount; i++) {
        s_projectiles[i].draw(renderer);
    }
}
