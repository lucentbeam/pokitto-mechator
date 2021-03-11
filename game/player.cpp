#include "player.h"

Player * Player::s_instance;

PlayerStats Player::s_stats;

Player::Player() :
    m_soldier(48*6, 13*6, 20.0f, 1.0f, {Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal}, 4, 4),
    m_jeep(26*6, 8*6, 50.0f, 0.1f, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 9, 9, 0.05f)
{
    m_shake.intensity = Rumbler::Slight;
    s_instance = this;
}

void Player::update(float dt) {
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    ControlStatus controls = Controls::getStatus(true);
    int damage = 0;
    if (m_mode == PlayerMode::Soldier) {
        m_soldier.update(dt, controls.x , controls.y);
        m_jeep.update(dt, 0.0f, 0.0f);
        damage = ProjectileManager::getCollisionDamage(m_soldier.rect(), bulletMask);
        if (damage > 0) {
            s_stats.health_soldier.change(-damage);
        }
    } else {
        m_jeep.update(dt, controls.x, controls.y);
        damage = ProjectileManager::getCollisionDamage(m_jeep.rect(), bulletMask);
        if (damage > 0) {
            s_stats.health_jeep.change(-damage);
        }
        if (m_jeep.moving()) {
            Terrain current = CollisionManager::getTerrainAt(m_jeep.pos().x(), m_jeep.pos().y());
            m_shake.intensity = current == Terrain::Grass ? Rumbler::Vigorous : Rumbler::Slight;
            m_jeep.scaleMaxSpeed(current == Terrain::Grass ? 0.5f : 1.0f);
            m_shake.update();
        }
    }
    if (controls.c.pressed()) {
        if (m_mode == PlayerMode::Soldier) {
            Vec2f dp = m_jeep.pos() - m_soldier.pos();
            if (dp.length() < 6) {
                m_mode = PlayerMode::Jeep;
            }
        } else {
            m_soldier.copyPosition(m_jeep);
            m_mode = PlayerMode::Soldier;
        }
    }
    if (m_mode == PlayerMode::Soldier) {
        const float shots_per_second = 3.0f;
        float frames_per_second = 1.0f / dt;
        float frames_per_shot = frames_per_second / shots_per_second;
        if (controls.a.downEvery(1, int(frames_per_shot))) {
            Projectile * p = ProjectileManager::create(m_soldier.pos(), m_soldier.aim() * 100.0f, 3, 0.5f)
                ->setSprite({projectile[0]}, 20)
                ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
        }
    } else {
        const float shots_per_second = 2.0f;
        float frames_per_second = 1.0f / dt;
        float frames_per_shot = frames_per_second / shots_per_second;
        if (controls.a.downEvery(1, int(frames_per_shot))) {
            Vec2f offset = m_jeep.aim().rot90();
            ProjectileManager::create(m_jeep.pos() + offset * 4.0f, m_jeep.aim() * 150.0f, 3, 0.35f)
                    ->setSprite({projectile[0]}, 20)
                    ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            ProjectileManager::create(m_jeep.pos() - offset * 4.0f, m_jeep.aim() * 150.0f, 3, 0.35f)
                    ->setSprite({projectile[0]}, 20)
                    ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
        }
        if (controls.b.pressed()) {

            ProjectileManager::create(m_jeep.pos(), m_jeep.vel() * 0.85f + m_jeep.facing() * 50.0f, 4, 0.5f)
                ->setSprite({projectile_grenade[0], projectile_grenade[1]}, 4)
                ->setTargetMask({EnemyTarget, GroundTarget})
                ->setDamage(0)
                ->setExpireCallback([](Projectile*p) {
                for(int i = -4; i <= 4; i+=4) {
                    for (int j = -4; j <= 4; j+= 4) {
                        Terrain t = CollisionManager::getTerrainAt(p->pos().x()+i, p->pos().y()+j);
                        if (t == Terrain::DestrucableWood) {
                            MapManager::setTileAt(p->pos().x()+i, p->pos().y()+j, 61);
                        }
                    }
                }
                ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls();
                EffectManager::create(p->pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
            });
        }
    }
}

void Player::draw() {
    // jeep rendering
    Vec2f jpos = Camera::worldToScreen(m_jeep.pos());
    RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - m_shake.offset(1), jeep[m_jeep.rotation_frame()], jeep[0][2], m_jeep.facing().x() > 0);

    if (m_mode == PlayerMode::Soldier) {
        // soldier rendering
        uint8_t sprite = m_soldier.facing().y() < 0 ? 3 : 0;
        static int counter = 0;
        if (m_soldier.moving()) {
            counter++;
            const int increment = 6;
            int mod = counter % (increment * 4);
            sprite += mod < increment ? 0 : mod < increment * 2 ? 1 : mod < increment * 3 ? 2 : 1;
        } else {
            sprite += 1;
        }
        Vec2f spos = Camera::worldToScreen(m_soldier.pos());
        RenderSystem::sprite(spos.x()- 3, spos.y() - 3, soldier[sprite], soldier[0][2], m_soldier.facing().x() > 0);
    }


}

Vec2f Player::position()
{
    return s_instance->m_mode == PlayerMode::Soldier ? s_instance->m_soldier.pos() : s_instance->m_jeep.pos();
}
