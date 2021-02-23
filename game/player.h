#ifndef PLAYER_H
#define PLAYER_H

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "game/rendering/camera.h"

#include "game/tilesets.h"
#include "game/physics/steering.h"
#include "game/entities/projectile.h"

class Player {
    Controls m_controller;

    Steering m_soldier;
    Steering m_jeep;

    bool m_dismounted = true;

public:
    Player(float x, float y);

    void update(float dt);

    void draw(RenderSystem * renderSystem);

    Vec2f pos() const;
};

#endif // PLAYER_H

Player::Player(float x, float y) :
    m_soldier(x, y, 20.0f, 1.0f, {Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal}, 4, 4),
    m_jeep(x + 6, y - 12, 50.0f, 0.1f, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 9, 9, 0.05f)
{

}

void Player::update(float dt) {
    ControlStatus controls = m_controller.getStatus(true);
    if (m_dismounted) {
        m_soldier.update(dt, controls.x , controls.y);
        m_jeep.update(dt, 0.0f, 0.0f);
    } else {
        m_jeep.update(dt, controls.x, controls.y);
    }
    if (controls.pc) {
        if (m_dismounted) {
            Vec2f dp = m_jeep.pos() - m_soldier.pos();
            if (dp.length() < 6) {
                m_dismounted = false;
            }
        } else {
            m_dismounted = true;
            m_soldier.copyPosition(m_jeep);
        }
    }
    if (controls.pa) {
        Vec2f p = !m_dismounted ? m_jeep.pos() : m_soldier.pos();
        Vec2f f = !m_dismounted ? m_jeep.aim() : m_soldier.aim();
        ProjectileManager::create(p, f * 25.0f, 3.5f)->setSprite({projectile[0]}, 20);
    }
}

void Player::draw(RenderSystem *renderSystem) {
    // jeep rendering
    Vec2f jpos = Camera::worldToScreen(m_jeep.pos());
    renderSystem->sprite(jpos.x() - 7, jpos.y() - 7, jeep[m_jeep.rotation_frame()], jeep[0][2], m_jeep.facing().x() > 0);

    if (m_dismounted) {
        // soldier rendering
        uint8_t sprite = m_soldier.facing().y() < 0 ? 3 : 0;
        static int counter = 0;
        if (m_soldier.moving()) {
            counter++;
            sprite += counter % 30 < 10 ? 0 : counter % 30 < 20 ? 1 : 2;
        } else {
            sprite += 1;
        }
        Vec2f spos = Camera::worldToScreen(m_soldier.pos());
        renderSystem->sprite(spos.x()- 3, spos.y() - 3, soldier[sprite], soldier[0][2], m_soldier.facing().x() > 0);
    }

}

Vec2f Player::pos() const
{
    return m_dismounted ? m_soldier.pos() : m_jeep.pos();
}
