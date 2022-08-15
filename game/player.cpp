#include "player.h"
#include "game/ui/ui.h"

#include "core/audiosystem.h"
#include "core/utilities/babyfsm.h"
#include "game/funcs.h"
#include "game/variables.h"

static uint32_t mode_switch_counter = 0;


WeaponHelper WeaponHelper::getIndexAndMax(int current, int owned)
{
    WeaponHelper h;
    for(int i = 0; i < 16; ++i) {
        if (current & 1 & owned) h.index = h.count;
        if (owned & 1) h.count++;
        current = current >> 1;
        owned = owned >> 1;
    }
    return h;
}

Vehicle::Vehicle(int8_t hp, float x, float y, const SteeringConfig * config) :
    m_steering(x, y, config),
    m_health(0, hp),
    m_aim(0, 0)
{

}

Soldier Soldier::s_instance;
#ifdef DEBUGS
int Soldier::s_owned_weapons = Weapon::Gun | Weapon::DualShot | Weapon::Missiles;
Weapon::Type Soldier::s_current_weapon = Weapon::MultiMissiles;
#else
int Soldier::s_owned_weapons = Weapon::Gun;
Weapon::Type Soldier::s_current_weapon = Weapon::Gun;
#endif

Jeep Jeep::s_instance;
int Jeep::s_owned_weapons = Weapon::DualShot | Weapon::Grenade;
Weapon::Type Jeep::s_current_weapon = Weapon::DualShot;

Tank Tank::s_instance;
int Tank::s_owned_weapons = Weapon::MachineGun | Weapon::Missiles;
Weapon::Type Tank::s_current_weapon = Weapon::Missiles;

Boat Boat::s_instance;
int Boat::s_owned_weapons = Weapon::MachineGun;
Weapon::Type Boat::s_current_weapon = Weapon::MachineGun;

Helicopter Helicopter::s_instance;
int Helicopter::s_owned_weapons = Weapon::MachineGun | Weapon::Missiles;
Weapon::Type Helicopter::s_current_weapon = Weapon::MachineGun;

PlayerMode Player::s_mode = PlayerMode::SoldierMode;

float Player::s_shot_cooldown = 0.0f;

const uint8_t reticle[] = {5, 5, 9, 9, 0, 9, 9, 9, 0, 0, 0, 9, 0, 0, 0, 0, 0, 9, 0, 0, 0, 9, 9, 9, 0, 9, 9 };
const float reticleDistance = 18.0f;

void Soldier::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    s_instance.updateFlash();
    if (Player::s_mode != PlayerMode::SoldierMode) return;
    ControlStatus controls = Controls::getStatus(true);
    s_instance.sprinting = controls.b.held();

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        s_instance.health().change(-damage);
        AudioSystem::play(sfxHit2);
        // TODO: gameover
    }

    s_instance.m_steering.update(dt, controls.x , controls.y, s_instance.sprinting ? 1.7f : 1.0f, true);

    if (!controls.a.held()) s_instance.m_aim = s_instance.m_steering.aim();

    if (Player::weaponCooldown(dt) && !s_instance.sprinting) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

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
        }
        else if (overlap_heli) {
            Player::s_mode = PlayerMode::HelicopterMode;
            Helicopter::launch();
            mode_switch_counter = 0;
            UI::showHealthbar();
        }
        else if (overlap_tank) {
            Player::s_mode = PlayerMode::TankMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
        }
        else if (overlap_boat) {
            Player::s_mode = PlayerMode::BoatMode;
            mode_switch_counter = 0;
            UI::showHealthbar();
        }
    }
}

void Soldier::draw()
{
    if (s_instance.m_z != 0) return;
    if (Player::s_mode != PlayerMode::SoldierMode) return;
    uint8_t sprite = s_instance.m_aim.y() < 0 ? 3 : 0;
    static int counter = 0;
    if (!FSM::instance->is(GameStates::Pause)) {
        if (s_instance.m_steering.moving()) {
            counter++;
            int increment = Controls::getStatus().b.held() ? 4 : 6;
            int mod = counter % (increment * 4);
            sprite += mod < increment ? 0 : mod < increment * 2 ? 1 : mod < increment * 3 ? 2 : 1;
        } else {
            sprite += 1;
        }
    }
    Vec2f spos = Camera::worldToScreen(s_instance.m_steering.pos());
    bool flip = s_instance.m_steering.facing().x() > 0.1f;
    if (s_instance.flashing()) {
        RenderSystem::sprite(spos.x()-(flip ? 4 : 3), spos.y() - 3, soldier[sprite], soldier[0][2], 10, flip);
    } else {
        RenderSystem::sprite(spos.x()-(flip ? 4 : 3), spos.y() - 3, soldier[sprite], soldier[0][2], flip);
    }
    Player::drawReticle(SoldierMode, s_instance.m_aim);
}

void Soldier::drawAir()
{
    if (s_instance.m_z == 0) return;
    Vec2f spos = Camera::worldToScreen(s_instance.m_steering.pos());

    RenderSystem::drawShadow(spos.x() - 6, spos.y() - 6, parachute, parachute[2]);

    RenderSystem::sprite(spos.x() - 3, spos.y() - 3 - s_instance.m_z, soldier[1], soldier[0][2], 1, false);
    RenderSystem::sprite(spos.x() - 6, spos.y() - 11 - s_instance.m_z, parachute, parachute[2]);
}

void Jeep::update(float dt)
{
    s_instance.updateFlash();
    if (Player::s_mode != PlayerMode::JeepMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);
    Soldier::setPosition(position());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        AudioSystem::play(sfxHit2);
        s_instance.health().change(-damage);
        if (!alive()) {
            onVehicleDestroyed();
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

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    Player::updateCounter();
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
    if (s_instance.flashing()) {
        RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
    } else {
        RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], s_instance.m_steering.facing().x() > 0.1f);
    }
    Player::drawReticle(JeepMode, s_instance.m_aim);
}


void Helicopter::update(float dt)
{
    s_instance.updateFlash();
    Player::updateCounter();
    if (!s_instance.m_inAir && s_instance.m_z > 0.0f) {
        s_instance.m_z -= 20.0f * dt;
        if (s_instance.m_z < 0.1f) {
            s_instance.m_z = 0.0f;
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

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::AirTarget});

    if (Player::s_mode != PlayerMode::HelicopterMode) {
        return;
    }

    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);
    Soldier::setPosition(s_instance.m_steering.pos());

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos() - Vec2f(0, s_instance.m_z), s_instance.m_aim, s_instance.m_steering.vel(), true);

    Rect rect = s_instance.m_steering.rect();
    rect.shift(0, -s_instance.m_z);
    int damage = ProjectileManager::getCollisionDamage(rect, bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        AudioSystem::play(sfxHit2);
        s_instance.health().change(-damage);
        if (!alive()) {
            onVehicleDestroyed();
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
    RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0.1f);
    RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[0], helicopter_blades[0][2]);
}

void Helicopter::drawAir()
{
    if (!alive() || s_instance.m_z < 0.01f) return;
    Vec2f pos = Camera::worldToScreen(position());
    RenderSystem::drawShadow(pos.x() - 9, pos.y() - 9 + s_instance.m_z, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0.1f);
    if (s_instance.flashing()) {
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
        if ((mode_switch_counter % 3) == 2) {
            RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 12)/3], helicopter_blades[0][2], 10, false);
        }
    } else {
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0.1f);
        if ((mode_switch_counter % 3) == 2) {
            RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 12)/6 * 2], helicopter_blades[0][2]);
        }
    }
    Player::drawReticle(HelicopterMode, s_instance.m_aim);
}

void Tank::update(float dt)
{
    s_instance.updateFlash();
    if (Player::s_mode != PlayerMode::TankMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f);
        return;
    }
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);
    Soldier::setPosition(position());

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        AudioSystem::play(sfxHit2);
        s_instance.health().change(-damage);
        if (!alive()) {
            onVehicleDestroyed();
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
        }
    }
    if (s_instance.m_steering.moving()) {
        s_instance.m_shake.intensity = Rumbler::Vigorous;
        s_instance.m_shake.update();
    }

    Player::updateCounter();
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
    if (s_instance.flashing()) {
        RenderSystem::sprite(pos.x() - 10, pos.y() - 10 - s_instance.m_shake.offset(1), tank[s_instance.m_steering.rotation_frame() + offset], tank[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
    } else {
        RenderSystem::sprite(pos.x() - 10, pos.y() - 10 - s_instance.m_shake.offset(1), tank[s_instance.m_steering.rotation_frame() + offset], tank[0][2], s_instance.m_steering.facing().x() > 0.1f);
    }
    Player::drawReticle(TankMode, s_instance.m_aim);
}

void Boat::update(float dt)
{
    s_instance.updateFlash();
    if (Player::s_mode != PlayerMode::BoatMode) {
        s_instance.m_steering.update(dt, 0.0f, 0.0f, 0.0f);
        return;
    }

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);
    Soldier::setPosition(position());

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        s_instance.health().change(-damage);
        AudioSystem::play(sfxHit2);
        if (!alive()) {
            // TODO: make gameover
            onVehicleDestroyed();
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
        }
    }

    static uint16_t disembarkPoints = Helpers::getMask({Terrain::None, Terrain::Mud, Terrain::Grass, Terrain::WaterShallow});
    Vec2f projection = position() + s_instance.m_steering.facing() * 12.0f;
    Player::updateCounter();
    if (controls.b.pressed() && mode_switch_counter > 1 && CollisionManager::collides(projection, disembarkPoints)) { // project forward and look for ground

        static uint16_t blocksLanding = Helpers::getMask({Terrain::Wall, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall});
        for (int i = 3; i <= 15; i += 3) {
            projection = position() + s_instance.m_steering.facing() * float(i);
            if (CollisionManager::collides(projection, blocksLanding)) {
                i = 18;
            } else if (CollisionManager::collides(projection, disembarkPoints)) {
                projection.setX(std::floor(projection.x() / 6.0f) * 6 + 3);
                projection.setY(std::floor(projection.y() / 6.0f) * 6 + 3);
                Soldier::setPosition(projection);
                Player::s_mode = PlayerMode::SoldierMode;
                mode_switch_counter = 0;
                UI::showHealthbar();
                i = 18;
            }
        }
    }
}

void Boat::draw()
{
    if (!alive()) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());
    if (s_instance.flashing()) {
        RenderSystem::sprite(pos.x() - 9.5f, pos.y() - 10, boat[s_instance.m_steering.rotation_frame()], boat[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
    } else {
        RenderSystem::sprite(pos.x() - 9.5f, pos.y() - 10, boat[s_instance.m_steering.rotation_frame()], boat[0][2], s_instance.m_steering.facing().x() > 0.1f);
    }
    Player::drawReticle(BoatMode, s_instance.m_aim);
}


bool Player::hurting()
{
    return Soldier::s_instance.flashing() || Helicopter::s_instance.flashing() || Jeep::s_instance.flashing() || Tank::s_instance.flashing() || Boat::s_instance.flashing();
}

bool Player::alive(PlayerMode m)
{
    switch(m) {
    case JeepMode:
        return Jeep::alive();
    case TankMode:
        return Tank::alive();
    case BoatMode:
        return Boat::alive();
    case HelicopterMode:
        return Helicopter::alive();
    }
    return true;
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

WeaponHelper Player::getCurrentWeaponInfo()
{
    switch (s_mode) {
    case PlayerMode::JeepMode:
        return WeaponHelper::getIndexAndMax(Jeep::s_current_weapon, Jeep::s_owned_weapons);
    case PlayerMode::HelicopterMode:
        return WeaponHelper::getIndexAndMax(Helicopter::s_current_weapon, Helicopter::s_owned_weapons);
    case PlayerMode::TankMode:
        return WeaponHelper::getIndexAndMax(Tank::s_current_weapon, Tank::s_owned_weapons);
    case PlayerMode::BoatMode:
        return WeaponHelper::getIndexAndMax(Boat::s_current_weapon, Boat::s_owned_weapons);
    default:
        return WeaponHelper::getIndexAndMax(Soldier::s_current_weapon, Soldier::s_owned_weapons);
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

void Player::drawReticle(PlayerMode mode, const Vec2f &dir)
{
    if (s_mode != mode) return;
    if (!FSM::instance->is(GameStates::Game)) return;
    static int fcounter = 0;
    fcounter++;
    if (!Controls::getStatus().a.held() || fcounter < 40) {
        auto spos = Vec2f(52, 42) + dir * reticleDistance;
        if ((fcounter % 50) < 6) RenderSystem::sprite(spos.x(), spos.y(), reticle, 0);
        else RenderSystem::drawShadow(spos.x(), spos.y(), reticle, 0);
        RenderSystem::pixel(spos.x() + 2, spos.y() + 2, 10);
    } else if (Controls::getStatus().a.holdCount() < 10) {
        fcounter = 0;
    }
}

void Player::drawFlashlight()
{
    Vec2f dir;
    float distance = 18.0f;
    bool doubled = false;
    switch(s_mode) {
    case PlayerMode::SoldierMode:
        distance = 5.0f;
        dir = Soldier::s_instance.m_aim;
        break;
    case PlayerMode::JeepMode:
        doubled = true;
        distance = 10.0f;
        dir = Jeep::s_instance.m_steering.facing();
        break;
    case PlayerMode::TankMode:
        doubled = true;
        distance = 12.0f;
        dir = Tank::s_instance.m_steering.facing();
        break;
    case PlayerMode::HelicopterMode:
        distance = 11.0f;
        dir = Helicopter::s_instance.m_steering.facing();
        break;
    case PlayerMode::BoatMode:
        distance = 11.0f;
        dir = Boat::s_instance.m_steering.facing();
        break;
    }
    int map_to = dir.getRotationFrame(8.0f);
    static const Vec2f mappings[] = {
        {0, 1},
        {0.383, 0.924},
        {0.707, 0.707},
        {0.924, 0.383},
        {1, 0},
        {0.924, -0.383},
        {0.707, -0.707},
        {0.383, -0.924},
        {0, -1}
    };
    bool flip = dir.x() < 0;
    dir = mappings[8 - map_to];
    if (flip) dir.setX(dir.x() * -1);

    map_to = dir.getRotationFrame(4.0f);
    static const Vec2i sprite_offsets[] = {
        {14, 27},
        {25, 25},
        {27, 13},
        {25, 2},
        {13, 0}
    };
    Vec2i sprite_offset = sprite_offsets[map_to];
    if (!flip) sprite_offset.setX(28 - sprite_offset.x());

    if (doubled) {
        Vec2f offset = dir.rot90() * 4.0f;
        RenderSystem::incrementColors(55 - sprite_offset.x() + dir.x() * distance + offset.x(), 44 - sprite_offset.y() + dir.y() * distance + offset.y(), flashlight[dir.getRotationFrame(4.0f)], 0, dir.x() > 0);
        RenderSystem::incrementColors(55 - sprite_offset.x() + dir.x() * distance - offset.x(), 44 - sprite_offset.y() + dir.y() * distance - offset.y(), flashlight[dir.getRotationFrame(4.0f)], 0, dir.x() > 0);
    } else {
        RenderSystem::incrementColors(55 - sprite_offset.x() + dir.x() * distance, 44 - sprite_offset.y() + dir.y() * distance, flashlight[dir.getRotationFrame(4.0f)], 0, dir.x() > 0);
    }
}

void Player::updateCounter()
{
    mode_switch_counter++;
}

void Player::storeData()
{
    GameStorage * dat = GameVariables::getData();

    dat->soldierPosition = Soldier::position() + Vec2f(0, 6);
    dat->jeepPosition =    Jeep::position() + (Player::mode() == JeepMode ? Vec2f(0, 6) : Vec2f(0,0));
    dat->boatPosition =    Boat::position();
    dat->heliPosition =    Helicopter::position();

    dat->soldierLife =     Soldier::s_instance.health().value();
    dat->jeepLife =        Jeep::s_instance.health().value();
    dat->boatLife =        Boat::s_instance.health().value();
    dat->heliLife =        Helicopter::s_instance.health().value();
}

void Player::loadData()
{
    GameStorage * dat = GameVariables::getData();
    Soldier::setPosition(dat->soldierPosition);
    Soldier::health().set(dat->soldierLife);
    Jeep::setPosition(dat->jeepPosition);
    Jeep::health().set(dat->jeepLife);
    Boat::setPosition(dat->boatPosition);
    Boat::health().set(dat->boatLife);
    Helicopter::setPosition(dat->heliPosition);
    Helicopter::health().set(dat->heliLife);
}

