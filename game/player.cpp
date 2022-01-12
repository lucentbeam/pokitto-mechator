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
int Soldier::s_owned_weapons = Weapon::Gun;
Weapon::Type Soldier::s_current_weapon = Weapon::Gun;

Jeep Jeep::s_instance;
int Jeep::s_owned_weapons = Weapon::DualShot | Weapon::Grenade;
Weapon::Type Jeep::s_current_weapon = Weapon::DualShot;

Tank Tank::s_instance;
int Tank::s_owned_weapons = Weapon::MachineGun | Weapon::Missiles;
Weapon::Type Tank::s_current_weapon = Weapon::MachineGun;

Boat Boat::s_instance;
int Boat::s_owned_weapons = Weapon::MachineGun;
Weapon::Type Boat::s_current_weapon = Weapon::MachineGun;

Helicopter Helicopter::s_instance;
int Helicopter::s_owned_weapons = Weapon::MachineGun | Weapon::Missiles;
Weapon::Type Helicopter::s_current_weapon = Weapon::MachineGun;

bool Jeep::s_available = true;

bool Helicopter::s_available = true;

bool Tank::s_available = true;

bool Boat::s_available = true;

PlayerMode Player::s_mode = PlayerMode::SoldierMode;

float Player::s_shot_cooldown = 0.0f;

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
    s_instance.m_iframes.update();
    if (damage > 0 && s_instance.m_iframes.ready()) {
        s_instance.m_iframes.reset(10);
        s_instance.health().change(-damage);
        // TODO: gameover
    }

    if (!controls.a.held()) {
        s_instance.m_aim = s_instance.m_steering.aim();
    }

    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    mode_switch_counter++;

    bool overlap_jeep = Jeep::alive() && (Jeep::position() - s_instance.m_steering.pos()).length() < 6;
    bool overlap_heli = Helicopter::alive() && (Helicopter::position() - s_instance.m_steering.pos()).length() < 6;
    bool overlap_tank = Tank::alive() && (Tank::position() - s_instance.m_steering.pos()).length() < 6;
    bool overlap_boat = Boat::alive() && (Boat::position() - s_instance.m_steering.pos()).length() < 18;
    s_instance.m_overlaps = overlap_jeep || overlap_heli || overlap_tank || overlap_boat;

    if (controls.b.pressed() && mode_switch_counter > 1) {
        if (overlap_jeep) {
            Player::s_mode = PlayerMode::JeepMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
        if (overlap_heli) {
            Player::s_mode = PlayerMode::HelicopterMode;
            Helicopter::launch();
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
        if (overlap_tank) {
            Player::s_mode = PlayerMode::TankMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
            s_instance.sprint_timer = 0.0f;
        }
        if (overlap_boat) {
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
    bool flip = s_instance.m_steering.facing().x() > 0;
    if (!s_instance.m_iframes.ready()) {
        RenderSystem::sprite(spos.x()-(flip ? 4 : 3), spos.y() - 3, soldier[sprite], soldier[0][2], 10, flip);
    } else {
        RenderSystem::sprite(spos.x()-(flip ? 4 : 3), spos.y() - 3, soldier[sprite], soldier[0][2], flip);
    }
}

void Jeep::update(float dt)
{
    s_instance.m_iframes.update();
    if (Player::s_mode != PlayerMode::JeepMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && s_instance.m_iframes.ready()) {
        s_instance.m_iframes.reset(10);
        s_instance.health().change(-damage);
        if (!alive()) {
            Soldier::setPosition(position());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
        }
    }
    if (s_instance.m_steering.moving()) {
        Terrain current = CollisionManager::getTerrainAt(s_instance.m_steering.pos().x(), s_instance.m_steering.pos().y());
        s_instance.m_shake.intensity = current == Terrain::Grass ? Rumbler::Vigorous : Rumbler::Slight;
        s_instance.m_steering.scaleMaxSpeed(current == Terrain::Grass ? jeepGrassSpeedFraction : 1.0f);
        s_instance.m_shake.update();
    }

    if (controls.a.pressed()) s_instance.m_aim = s_instance.m_steering.aim();


    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

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
    if (!s_instance.m_iframes.ready()) {
        RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], 10, s_instance.m_steering.facing().x() > 0);
    } else {
        RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], s_instance.m_steering.facing().x() > 0);
    }
}


void Helicopter::update(float dt)
{
    s_instance.m_iframes.update();
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

    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    Rect rect = s_instance.m_steering.rect();
    rect.shift(0, -s_instance.m_z);
    int damage = ProjectileManager::getCollisionDamage(rect, bulletMask);
    if (damage > 0 && s_instance.m_iframes.ready()) {
        s_instance.m_iframes.reset(10);
        s_instance.health().change(-damage);
        if (!alive()) {
            Soldier::setPosition(s_instance.m_steering.pos());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(position() - Vec2f(6,6));
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
    if (!s_instance.m_iframes.ready()) {
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], 10, s_instance.m_steering.facing().x() > 0);
        if ((mode_switch_counter % 3) == 2) {
            RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 12)/3], helicopter_blades[0][2], 10, false);
        }
    } else {
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0);
        if ((mode_switch_counter % 3) == 2) {
            RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 12)/3], helicopter_blades[0][2]);
        }
    }
}

void Tank::update(float dt)
{
    s_instance.m_iframes.update();
    if (Player::s_mode != PlayerMode::TankMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && s_instance.m_iframes.ready()) {
        s_instance.m_iframes.reset(10);
        s_instance.health().change(-damage);
        if (!alive()) {
            Soldier::setPosition(position());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
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
    if (!s_instance.m_iframes.ready()) {
        RenderSystem::sprite(pos.x() - 10, pos.y() - 10 - s_instance.m_shake.offset(1), tank[s_instance.m_steering.rotation_frame() + offset], tank[0][2], 10, s_instance.m_steering.facing().x() > 0);
    } else {
        RenderSystem::sprite(pos.x() - 10, pos.y() - 10 - s_instance.m_shake.offset(1), tank[s_instance.m_steering.rotation_frame() + offset], tank[0][2], s_instance.m_steering.facing().x() > 0);
    }
}

void Boat::update(float dt)
{
    s_instance.m_iframes.update();
    if (Player::s_mode != PlayerMode::BoatMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);

    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && s_instance.m_iframes.ready()) {
        s_instance.m_iframes.reset(10);
        s_instance.health().change(-damage);
        if (!alive()) {
            // TODO: make gameover
            Soldier::setPosition(position());
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
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
    if (!s_instance.m_iframes.ready()) {
        RenderSystem::sprite(pos.x() - 15, pos.y() - 15, boat[s_instance.m_steering.rotation_frame()], boat[0][2], 10, s_instance.m_steering.facing().x() > 0);
    } else {
        RenderSystem::sprite(pos.x() - 15, pos.y() - 15, boat[s_instance.m_steering.rotation_frame()], boat[0][2], s_instance.m_steering.facing().x() > 0);
    }
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

Weapon::Type Player::currentWeapon()
{
    switch (s_mode) {
    case PlayerMode::JeepMode:
        return Jeep::s_current_weapon;
    case PlayerMode::HelicopterMode:
        return Helicopter::s_current_weapon;
    case PlayerMode::TankMode:
        return Tank::s_current_weapon;
    case PlayerMode::BoatMode:
        return Boat::s_current_weapon;
    default:
        return Soldier::s_current_weapon;
    }
}

void Player::cycleWeaponNext()
{
    switch (s_mode) {
    case PlayerMode::JeepMode:
        Jeep::s_current_weapon = Weapon::nextWeapon(Jeep::s_current_weapon);
        while(!(Jeep::s_current_weapon & Jeep::s_owned_weapons)) {
            Jeep::s_current_weapon = Weapon::nextWeapon(Jeep::s_current_weapon);
        }
        break;
    case PlayerMode::HelicopterMode:
        Helicopter::s_current_weapon = Weapon::nextWeapon(Helicopter::s_current_weapon);
        while(!(Helicopter::s_current_weapon & Helicopter::s_owned_weapons)) {
            Helicopter::s_current_weapon = Weapon::nextWeapon(Helicopter::s_current_weapon);
        }
        break;
    case PlayerMode::TankMode:
        Tank::s_current_weapon = Weapon::nextWeapon(Tank::s_current_weapon);
        while(!(Tank::s_current_weapon & Tank::s_owned_weapons)) {
            Tank::s_current_weapon = Weapon::nextWeapon(Tank::s_current_weapon);
        }
        break;
    case PlayerMode::BoatMode:
        Boat::s_current_weapon = Weapon::nextWeapon(Boat::s_current_weapon);
        while(!(Boat::s_current_weapon & Boat::s_owned_weapons)) {
            Boat::s_current_weapon = Weapon::nextWeapon(Boat::s_current_weapon);
        }
        break;
    default:
        Soldier::s_current_weapon = Weapon::nextWeapon(Soldier::s_current_weapon);
        while(!(Soldier::s_current_weapon & Soldier::s_owned_weapons)) {
            Soldier::s_current_weapon = Weapon::nextWeapon(Soldier::s_current_weapon);
        }
        break;
    }
}

void Player::cycleWeaponPrev()
{
    switch (s_mode) {
    case PlayerMode::JeepMode:
        Jeep::s_current_weapon = Weapon::prevWeapon(Jeep::s_current_weapon);
        while(!(Jeep::s_current_weapon & Jeep::s_owned_weapons)) {
            Jeep::s_current_weapon = Weapon::prevWeapon(Jeep::s_current_weapon);
        }
        break;
    case PlayerMode::HelicopterMode:
        Helicopter::s_current_weapon = Weapon::prevWeapon(Helicopter::s_current_weapon);
        while(!(Helicopter::s_current_weapon & Helicopter::s_owned_weapons)) {
            Helicopter::s_current_weapon = Weapon::prevWeapon(Helicopter::s_current_weapon);
        }
        break;
    case PlayerMode::TankMode:
        Tank::s_current_weapon = Weapon::prevWeapon(Tank::s_current_weapon);
        while(!(Tank::s_current_weapon & Tank::s_owned_weapons)) {
            Tank::s_current_weapon = Weapon::prevWeapon(Tank::s_current_weapon);
        }
        break;
    case PlayerMode::BoatMode:
        Boat::s_current_weapon = Weapon::prevWeapon(Boat::s_current_weapon);
        while(!(Boat::s_current_weapon & Boat::s_owned_weapons)) {
            Boat::s_current_weapon = Weapon::prevWeapon(Boat::s_current_weapon);
        }
        break;
    default:
        Soldier::s_current_weapon = Weapon::prevWeapon(Soldier::s_current_weapon);
        while(!(Soldier::s_current_weapon & Soldier::s_owned_weapons)) {
            Soldier::s_current_weapon = Weapon::prevWeapon(Soldier::s_current_weapon);
        }
        break;
    }
}

bool Player::weaponCooldown(float dt)
{
    s_shot_cooldown -= dt;
    if (s_shot_cooldown <= 0) {
        s_shot_cooldown = 0;
        return true;
    }
    return false;
}
