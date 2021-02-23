#include "projectile.h"
#include "game/rendering/camera.h"

Projectile::Projectile() :
    m_body({0, 0}, {0, 0}),
    m_lifetime(0),
    m_hit_wall(nullptr),
    m_hit_entity(nullptr)
{

}

Projectile::Projectile(const Vec2f &pos, const Vec2f &vel, float lifetime) :
    m_body(pos, vel),
    m_lifetime(lifetime),
    m_hit_wall(nullptr),
    m_hit_entity(nullptr)
{

}

void Projectile::setWallCollide(void (*wall_collide_callback)(Projectile *, uint16_t))
{
    m_hit_wall = wall_collide_callback;
}

void Projectile::setEntityCollide(void (*enemy_collide_callback)(Projectile *, uint16_t, void *))
{
    m_hit_entity = enemy_collide_callback;
}

void Projectile::update(float dt)
{
    m_body.update(dt);
    m_lifetime -= dt;
}

void Projectile::draw(RenderSystem *renderer)
{
    Vec2f pos = Camera::worldToScreen(m_body.pos());
    renderer->drawRect(pos.x()-1,pos.y(),2,2,9);
    renderer->drawRect(pos.x()-1,pos.y()-1,2,2,41);
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
    int i = 0;
    while (i < s_projectileCount) {
        s_projectiles[i].update(dt);
        if (s_projectiles[i].expired()) {
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
