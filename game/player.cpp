#include "player.h"
#include "game/ui/ui.h"

static uint32_t mode_switch_counter = 0;

Vehicle::Vehicle(int8_t hp, float x, float y, float speed, float cornering, std::initializer_list<uint8_t> collisions, float w, float h, float friction) :
    m_steering(x, y, speed, cornering, collisions, w, h, friction),
    m_health(0, hp)
{

}

Soldier Soldier::s_instance;

Jeep Jeep::s_instance;

bool Jeep::s_available = true;

PlayerMode Player::s_mode = PlayerMode::SoldierMode;

void Soldier::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    if (Player::s_mode != PlayerMode::SoldierMode) return;
    ControlStatus controls = Controls::getStatus(true);

    s_instance.m_steering.update(dt, controls.x , controls.y);
    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0) {
        s_instance.health().change(-damage);
    }

    const float shots_per_second = 3.0f;
    float frames_per_second = 1.0f / dt;
    float frames_per_shot = frames_per_second / shots_per_second;
    if (controls.a.downEvery(1, int(frames_per_shot))) {
        Projectile * p = ProjectileManager::create(s_instance.m_steering.pos(), s_instance.m_steering.aim() * 100.0f, 3, 0.5f)
            ->setSprite({projectile[0]}, 20)
            ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
    }

    mode_switch_counter++;
    if (Jeep::alive() && controls.c.releasedWithin(60) && mode_switch_counter > 1) {
        Vec2f dp = Jeep::position() - s_instance.m_steering.pos();
        if (dp.length() < 6) {
            Player::s_mode = PlayerMode::JeepMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
        }
    }
}

void Soldier::draw()
{
    if (Player::s_mode != PlayerMode::SoldierMode) return;
    uint8_t sprite = s_instance.m_steering.facing().y() < 0 ? 3 : 0;
    static int counter = 0;
    if (s_instance.m_steering.moving()) {
        counter++;
        const int increment = 6;
        int mod = counter % (increment * 4);
        sprite += mod < increment ? 0 : mod < increment * 2 ? 1 : mod < increment * 3 ? 2 : 1;
    } else {
        sprite += 1;
    }
    Vec2f spos = Camera::worldToScreen(s_instance.m_steering.pos());
    RenderSystem::sprite(spos.x()- 3, spos.y() - 3, soldier[sprite], soldier[0][2], s_instance.m_steering.facing().x() > 0);
}

void Jeep::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    if (Player::s_mode != PlayerMode::JeepMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0) {
        s_instance.health().change(-damage);
        if (!alive()) {
            s_instance.m_steering.copyPosition(s_instance.m_steering);
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::create(s_instance.m_steering.pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
        }
    }
    if (s_instance.m_steering.moving()) {
        Terrain current = CollisionManager::getTerrainAt(s_instance.m_steering.pos().x(), s_instance.m_steering.pos().y());
        s_instance.m_shake.intensity = current == Terrain::Grass ? Rumbler::Vigorous : Rumbler::Slight;
        s_instance.m_steering.scaleMaxSpeed(current == Terrain::Grass ? 0.5f : 1.0f);
        s_instance.m_shake.update();
    }

    const float shots_per_second = 2.0f;
    float frames_per_second = 1.0f / dt;
    float frames_per_shot = frames_per_second / shots_per_second;
    if (controls.a.downEvery(1, int(frames_per_shot))) {
        Vec2f offset = s_instance.m_steering.aim().rot90();
        ProjectileManager::create(s_instance.m_steering.pos() + offset * 4.0f, s_instance.m_steering.aim() * 150.0f, 3, 0.35f)
                ->setSprite({projectile[0]}, 20)
                ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
        ProjectileManager::create(s_instance.m_steering.pos() - offset * 4.0f, s_instance.m_steering.aim() * 150.0f, 3, 0.35f)
                ->setSprite({projectile[0]}, 20)
                ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
    }
    if (controls.b.pressed()) {
        ProjectileManager::create(s_instance.m_steering.pos(), s_instance.m_steering.vel() * 0.85f + s_instance.m_steering.facing() * 50.0f, 4, 0.5f)
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

    mode_switch_counter++;
    if (controls.c.releasedWithin(60) && mode_switch_counter > 1) {
        Soldier::setPosition(position());
        Player::s_mode = PlayerMode::SoldierMode;
        mode_switch_counter = 0;
        UI::showHealthbar();
    }
}

void Jeep::draw()
{
    if (!alive()) return;
    Vec2f jpos = Camera::worldToScreen(s_instance.m_steering.pos());
    RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], s_instance.m_steering.facing().x() > 0);
}

Vec2f Player::position()
{
    switch (s_mode) {
    case PlayerMode::JeepMode:
        return Jeep::position();
    default:
        return Soldier::position();
    }
}
