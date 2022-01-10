#include "player.h"
#include "game/ui/ui.h"

static uint32_t mode_switch_counter = 0;

Vehicle::Vehicle(int8_t hp, float x, float y, float speed, float cornering, std::initializer_list<uint8_t> collisions, float w, float h, float friction) :
    m_steering(x, y, speed, cornering, collisions, w, h, friction),
    m_health(0, hp),
    m_aim(0, 0)
{

}

Soldier Soldier::s_instance;

Jeep Jeep::s_instance;

Tank Tank::s_instance;

Boat Boat::s_instance;

Helicopter Helicopter::s_instance;

bool Jeep::s_available = true;

bool Helicopter::s_available = true;

bool Tank::s_available = true;

bool Boat::s_available = true;

PlayerMode Player::s_mode = PlayerMode::SoldierMode;

void Soldier::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    if (Player::s_mode != PlayerMode::SoldierMode) return;
    ControlStatus controls = Controls::getStatus(true);

    static bool running = false;
    if (controls.b.pressed()) {
        if (s_instance.sprint_timer < 1.0f) {
            running = true;
        }
    }
    if (controls.b.held() && running) {
        s_instance.sprint_timer += 1.0f / sprint_duration * dt;
        if (s_instance.sprint_timer > 1.0f) {
            running = false;
            s_instance.sprint_timer = 1.0f;
        }
    } else {
        running = false;
        s_instance.sprint_timer -= 1.0f / sprint_cooldown * dt;
        if (s_instance.sprint_timer < 0.0f) s_instance.sprint_timer = 0.0f;
    }



    s_instance.m_steering.update(dt, controls.x , controls.y, running ? 2.2f : 1.0f);
    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0) {
        s_instance.health().change(-damage);
    }

    if (!controls.a.held()) {
        s_instance.m_aim = s_instance.m_steering.aim();
    }

    const float shots_per_second = 1.6f;
    float frames_per_second = 1.0f / dt;
    float frames_per_shot = frames_per_second / shots_per_second;
    if (controls.a.downEvery(1, int(frames_per_shot))) {
        Projectile * p = ProjectileManager::create(s_instance.m_steering.pos(), s_instance.m_aim * 100.0f, 3, 0.5f)
            ->setSprite({projectile[0]}, 20)
            ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
    }

    mode_switch_counter++;

    if (controls.b.pressed() && mode_switch_counter > 1) {
        Vec2f dp = Jeep::position() - s_instance.m_steering.pos();
        if (Jeep::alive() && dp.length() < 6) {
            Player::s_mode = PlayerMode::JeepMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
        dp = Helicopter::position() - s_instance.m_steering.pos();
        if (Helicopter::alive() && dp.length() < 6) {
            Player::s_mode = PlayerMode::HelicopterMode;
            Helicopter::launch();
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
        dp = Tank::position() - s_instance.m_steering.pos();
        if (Tank::alive() && dp.length() < 6) {
            Player::s_mode = PlayerMode::TankMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
        dp = Boat::position() - s_instance.m_steering.pos();
        if (Boat::alive() && dp.length() < 18) {
            Player::s_mode = PlayerMode::BoatMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
    }
}

void Soldier::draw()
{
    if (Player::s_mode != PlayerMode::SoldierMode) return;
    uint8_t sprite = s_instance.m_aim.y() < 0 ? 3 : 0;
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
            Soldier::setPosition(position());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::create(s_instance.m_steering.pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
        }
    }
    if (s_instance.m_steering.moving()) {
        Terrain current = CollisionManager::getTerrainAt(s_instance.m_steering.pos().x(), s_instance.m_steering.pos().y());
        s_instance.m_shake.intensity = current == Terrain::Grass ? Rumbler::Vigorous : Rumbler::Slight;
        s_instance.m_steering.scaleMaxSpeed(current == Terrain::Grass ? jeepGrassSpeedFraction : 1.0f);
        s_instance.m_shake.update();
    }

    const float shots_per_second = 2.2f;
    float frames_per_second = 1.0f / dt;
    float frames_per_shot = frames_per_second / shots_per_second;
    if (controls.a.pressed()) {
        s_instance.m_aim = Vec2f(controls.x, controls.y);
    }
    if (controls.a.downEvery(1, int(frames_per_shot))) {
        Vec2f offset = s_instance.m_aim.rot90();
        ProjectileManager::create(s_instance.m_steering.pos() + offset * 3.0f, s_instance.m_aim * 150.0f, 3, 0.35f)
                ->setSprite({projectile[0]}, 20)
                ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
        ProjectileManager::create(s_instance.m_steering.pos() - offset * 3.0f, s_instance.m_aim * 150.0f, 3, 0.35f)
                ->setSprite({projectile[0]}, 20)
                ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
    }

    //    s_instance.m_steering.setBrake(controls.b.held());
//    if (controls.b.pressed()) {
//        ProjectileManager::create(s_instance.m_steering.pos(), s_instance.m_steering.vel() * 0.85f + s_instance.m_steering.facing() * 50.0f, 4, 0.5f)
//            ->setSprite({projectile_grenade[0], projectile_grenade[1]}, 4)
//            ->setTargetMask({EnemyTarget, GroundTarget})
//            ->setDamage(0)
//            ->setExpireCallback([](Projectile*p) {
//            for(int i = -4; i <= 4; i+=4) {
//                for (int j = -4; j <= 4; j+= 4) {
//                    Terrain t = CollisionManager::getTerrainAt(p->pos().x()+i, p->pos().y()+j);
//                    if (t == Terrain::DestrucableWood) {
//                        MapManager::setTileAt(p->pos().x()+i, p->pos().y()+j, 61);
//                    }
//                }
//            }
//            ProjectileManager::create(p->pos(), {0, 0}, 10, 0.1)->setDamage(3)->setIgnoreWalls();
//            EffectManager::create(p->pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
//        });
//    }

    mode_switch_counter++;
    if (controls.b.pressed() && mode_switch_counter > 1) {
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
    case PlayerMode::HelicopterMode:
        return Helicopter::position();
    case PlayerMode::TankMode:
        return Tank::position();
    case PlayerMode::BoatMode:
        return Boat::position();
    default:
        return Soldier::position();
    }
}

Rect Player::bounds()
{
    switch (s_mode) {
    case PlayerMode::JeepMode:
        return Jeep::bounds();
    case PlayerMode::HelicopterMode:
        return Helicopter::bounds();
    case PlayerMode::TankMode:
        return Tank::bounds();
    case PlayerMode::BoatMode:
        return Boat::bounds();
    default:
        return Soldier::bounds();
    }
}

void Helicopter::update(float dt)
{
    mode_switch_counter++;
    if (!s_instance.m_inAir && s_instance.m_z > 0.0f) {
        s_instance.m_z -= 20.0f * dt;
        if (s_instance.m_z < 0.1f) {
            s_instance.m_z = 0.0f;
            Soldier::setPosition(s_instance.m_steering.pos());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            s_instance.m_steering.stop();
        }
        return;
    } else if (s_instance.m_inAir && s_instance.m_z < 20.0f) {
        s_instance.m_z += 20.0f * dt;
        if (s_instance.m_z > 20.0f) {
            s_instance.m_z = 20.0f;
        }
        return;
    }

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    if (Player::s_mode != PlayerMode::HelicopterMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0) {
        s_instance.health().change(-damage);
        if (!alive()) {
            Soldier::setPosition(s_instance.m_steering.pos());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::create(position() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
        }
    }
    if (controls.b.pressed()) { // TODO: check valid landing loc
        s_instance.m_inAir = false;
    }
}

void Helicopter::drawGround()
{
    if (!alive() || s_instance.m_z > 0.0f) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());
    RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0);
    RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[0], helicopter_blades[0][2]);
}

void Helicopter::drawAir()
{
    if (!alive() || s_instance.m_z < 0.01f) return;
    Vec2f pos = Camera::worldToScreen(position());
    RenderSystem::drawShadow(pos.x() - 9, pos.y() - 9 + s_instance.m_z, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0);
    RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0);
    if ((mode_switch_counter % 3) == 2) {
        RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 12)/3], helicopter_blades[0][2]);
    }
}

void Tank::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    if (Player::s_mode != PlayerMode::TankMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0) {
        s_instance.health().change(-damage);
        if (!alive()) {
            Soldier::setPosition(position());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::create(s_instance.m_steering.pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
        }
    }
    if (s_instance.m_steering.moving()) {
        s_instance.m_shake.intensity = Rumbler::Vigorous;
        s_instance.m_shake.update();
    }

    mode_switch_counter++;
    if (controls.b.pressed() && mode_switch_counter > 1) {
        Soldier::setPosition(position());
        Player::s_mode = PlayerMode::SoldierMode;
        mode_switch_counter = 0;
        UI::showHealthbar();
    }
}

void Tank::draw()
{
    if (!alive()) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());
    int offset = (mode_switch_counter % 30) < 15 && s_instance.m_steering.moving() ? 9 : 0;
    RenderSystem::sprite(pos.x() - 10, pos.y() - 10 - s_instance.m_shake.offset(1), tank[s_instance.m_steering.rotation_frame() + offset], tank[0][2], s_instance.m_steering.facing().x() > 0);
}

void Boat::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    if (Player::s_mode != PlayerMode::BoatMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0) {
        s_instance.health().change(-damage);
        if (!alive()) {
            // TODO: make gameover
            Soldier::setPosition(position());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::create(s_instance.m_steering.pos() - Vec2f(6,6), {explosion[0], explosion[1], explosion[2], explosion[3], explosion[4], explosion[5], explosion[6]}, 40.0f);
        }
    }

    static uint16_t disembarkPoints = Helpers::getMask({Terrain::None, Terrain::Mud, Terrain::Grass, Terrain::WaterShallow});
    Vec2f projection = position() + s_instance.m_steering.facing() * 15.0f;
    mode_switch_counter++;
    if (controls.b.pressed() && mode_switch_counter > 1 && CollisionManager::collides(projection, disembarkPoints)) { // project forward and look for ground
        Soldier::setPosition(projection);
        Player::s_mode = PlayerMode::SoldierMode;
        mode_switch_counter = 0;
        UI::showHealthbar();
    }
}

void Boat::draw()
{
    if (!alive()) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());
    RenderSystem::sprite(pos.x() - 15, pos.y() - 15, boat[s_instance.m_steering.rotation_frame()], boat[0][2], s_instance.m_steering.facing().x() > 0);
}
