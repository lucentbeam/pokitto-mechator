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

Vehicle::Vehicle(int8_t hp, float x, float y, const SteeringConfig * config, int weapons, Weapon::Type default_weapon) :
    m_steering(x, y, config),
    m_health(0, hp),
    m_aim(1, 0),
    m_valid_weapons(weapons),
    current_weapon(default_weapon)
{

}

Soldier Soldier::s_instance;
Jeep Jeep::s_instance;
Tank Tank::s_instance;
Boat Boat::s_instance;
Helicopter Helicopter::s_instance;

PlayerMode Player::s_mode = PlayerMode::SoldierMode;

int Player::s_owned_weapons = Weapon::Gun | Weapon::Grenade | Weapon::DualShot | Weapon::Missiles;

float Player::s_shot_cooldown = 0.0f;

const uint8_t reticle[] = {5, 5, 9, 9, 0, 9, 9, 9, 0, 0, 0, 9, 0, 0, 0, 0, 0, 9, 0, 0, 0, 9, 9, 9, 0, 9, 9 };
const float reticleDistance = 18.0f;

static int8_t no_heli_land = 0;

void Soldier::update(float dt)
{
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    s_instance.updateFlash();
    if (Player::s_mode != PlayerMode::SoldierMode) return;
    ControlStatus controls = Controls::getStatus(true);
    s_instance.sprinting = controls.b.held();

    if (s_instance.m_health.max() == soldierHealth && GameVariables::hasBlueprintUnlocked(Blueprints::TacticalArmorBP)) {
        s_instance.m_health.setMaxto(soldierHealthArmored);
    }

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        s_instance.m_health.change(-damage);
        AudioSystem::play(sfxHit2);
    }

    s_instance.m_steering.update(dt, controls.x , controls.y, s_instance.sprinting ? GameVariables::hasBlueprintUnlocked(Blueprints::NinjaShoesBP) ? 2.4f : 1.55f : 1.0f, true);

    if (!controls.a.held() || s_instance.sprinting) s_instance.m_aim = s_instance.m_steering.aim();

    if (Player::weaponCooldown(dt) && !s_instance.sprinting) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_instance.current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    mode_switch_counter++;

    {
        s_instance.m_overlaps = PlayerMode::SoldierMode;
        const int sizes[4] = {6, 8, 10, 14};
        const PlayerMode prefs[4] = {JeepMode, HelicopterMode, TankMode, BoatMode};
        for(int i = 0; i < 4; ++i) {
            if (Player::alive(prefs[i]) && (Player::position(prefs[i]) - s_instance.m_steering.pos()).length() < sizes[i]) {
                s_instance.m_overlaps = prefs[i];
                break;
            }
        }
    }

    if (s_instance.m_overlaps != SoldierMode && controls.b.pressed() && mode_switch_counter > 1) {
        Player::s_mode = s_instance.m_overlaps;
        mode_switch_counter = 0;
        UI::showHealthbar();
        if (Player::s_mode == HelicopterMode) Helicopter::launch();
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
    Player::setPosition(SoldierMode, s_instance.m_steering.pos());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        AudioSystem::play(sfxHit2);
        s_instance.m_health.change(-damage);
        if (!Player::alive(JeepMode)) {
            onVehicleDestroyed();
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
        }
    }
    if (s_instance.m_steering.moving()) {
        Terrain current = CollisionManager::getTerrainAt(s_instance.m_steering.pos().x(), s_instance.m_steering.pos().y());
        s_instance.m_shake.intensity = current == Terrain::Grass ? Rumbler::Vigorous : Rumbler::Slight;
        s_instance.m_steering.scaleMaxSpeed(current == Terrain::Grass && !GameVariables::hasBlueprintUnlocked(AllTerrainTiresBP) ? jeepGrassSpeedFraction : 1.0f);
        s_instance.m_shake.update();
    }

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_instance.current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    Player::updateCounter();
    if (controls.b.pressed() && mode_switch_counter > 1) {
        Player::s_mode = PlayerMode::SoldierMode;
        mode_switch_counter = 0;
        UI::showHealthbar();
    }
}

void Jeep::draw()
{
    if (!Player::alive(JeepMode)) return;
    Vec2f jpos = Camera::worldToScreen(s_instance.m_steering.pos());
    if (Player::s_mode == SoldierMode && Soldier::overlaps(JeepMode)) {
        int f = s_instance.m_steering.rotation_frame();
        bool flip = s_instance.m_steering.facing().x() > 0.1f;
        RenderSystem::sprite(jpos.x() - 6, jpos.y() - 6, jeep[f], jeep[0][2], 10, flip);
        RenderSystem::sprite(jpos.x() - 8, jpos.y() - 6, jeep[f], jeep[0][2], 10, flip);
        RenderSystem::sprite(jpos.x() - 8, jpos.y() - 8, jeep[f], jeep[0][2], 10, flip);
        RenderSystem::sprite(jpos.x() - 6, jpos.y() - 8, jeep[f], jeep[0][2], 10, flip);
    }
    if (s_instance.flashing()) {
        RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
    } else {
        RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_instance.m_shake.offset(1), jeep[s_instance.m_steering.rotation_frame()], jeep[0][2], s_instance.m_steering.facing().x() > 0.1f);
    }
    Player::drawReticle(JeepMode, s_instance.m_aim);
}


void Helicopter::launch() { s_instance.m_inAir = true; }

bool Helicopter::active() { return Player::alive(HelicopterMode) && s_instance.m_inAir && s_instance.m_z >= 20.0f; }

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

    if (s_instance.m_health.max() == helicopterHealth && GameVariables::hasBlueprintUnlocked(Blueprints::UnobtainiumHullBP)) {
        s_instance.m_health.setMaxto(helicopterHealthArmored);
    }

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::AirTarget});

    if (no_heli_land > 0) no_heli_land--;

    if (Player::s_mode != PlayerMode::HelicopterMode) {
        return;
    }

    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);
    Player::setPosition(SoldierMode, s_instance.m_steering.pos());

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_instance.current_weapon, s_instance.m_steering.pos() - Vec2f(0, s_instance.m_z), s_instance.m_aim, s_instance.m_steering.vel(), true);

    Rect rect = s_instance.m_steering.rect();
    rect.shift(0, -s_instance.m_z);
    int damage = ProjectileManager::getCollisionDamage(rect, bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        AudioSystem::play(sfxHit2);
        s_instance.m_health.change(-damage);
        if (!Player::alive(HelicopterMode)) {
            onVehicleDestroyed();
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
        }
    }
    if (controls.b.pressed()) {
        for(int i = -1; i <= 1; i += 2) {
            for(int j = -1; j <= 1; j+= 2) {
                if (CollisionManager::collides(s_instance.m_steering.pos() + Vec2f(i * 3, j * 3), steering_soldier.collisions)) {
                    AudioSystem::play(sfxDeny);
                    no_heli_land = 30;
                    return;
                }
            }
        }
        s_instance.m_inAir = false;
    }
}

void Helicopter::drawGround()
{
    if (!Player::alive(HelicopterMode) || s_instance.m_z > 0.0f) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());
    if (Player::s_mode == SoldierMode && Soldier::overlaps(HelicopterMode)) {
        int f = s_instance.m_steering.rotation_frame();
        bool flip = s_instance.m_steering.facing().x() > 0.1f;
        RenderSystem::sprite(pos.x() - 10, pos.y() - 8, helicopter[f], helicopter[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 10, pos.y() - 10, helicopter[f], helicopter[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 8, pos.y() - 8, helicopter[f], helicopter[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 8, pos.y() - 10, helicopter[f], helicopter[0][2], 10, flip);
    }
    RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0.1f);
    RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[0], helicopter_blades[0][2]);
}

void Helicopter::drawAir()
{
    if (!Player::alive(HelicopterMode) || s_instance.m_z < 0.01f) return;
    Vec2f pos;
    if (no_heli_land > 0 && (no_heli_land % 8) < 3) {
        pos = Camera::worldToScreen(s_instance.m_steering.pos());
        RenderSystem::drawRect(pos.x() - 3, pos.y() - 3, 8, 6, 16);
    }
    pos = Camera::worldToScreen(s_instance.m_steering.pos() + Vec2f(0, -s_instance.m_z));
    RenderSystem::drawShadow(pos.x() - 9, pos.y() - 9 + s_instance.m_z, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0.1f);
    if (s_instance.flashing()) {
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
        if ((mode_switch_counter % 3) == 2) {
            RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 6)/3], helicopter_blades[0][2], 10, false);
        }
    } else {
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, helicopter[s_instance.m_steering.rotation_frame()], helicopter[0][2], s_instance.m_steering.facing().x() > 0.1f);
        if ((mode_switch_counter % 3) == 2) {
            RenderSystem::sprite(pos.x() - 9 + (s_instance.m_steering.facing().x() > 0 ? 1 : 0), pos.y() - 9, helicopter_blades[1 + (mode_switch_counter % 6)/3], helicopter_blades[0][2]);
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
    Player::setPosition(SoldierMode, s_instance.m_steering.pos());

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_instance.current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        AudioSystem::play(sfxHit2);
        s_instance.m_health.change(-damage);
        if (!Player::alive(TankMode)) {
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
        Player::s_mode = PlayerMode::SoldierMode;
        mode_switch_counter = 0;
        UI::showHealthbar();
    }
}

void Tank::draw()
{
    if (!Player::alive(TankMode)) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());
    int offset = (mode_switch_counter % 30) < 15 && s_instance.m_steering.moving() ? 9 : 0;
    if (Player::s_mode == SoldierMode && Soldier::overlaps(TankMode)) {
        int f = s_instance.m_steering.rotation_frame() + offset;
        bool flip = s_instance.m_steering.facing().x() > 0.1f;
        RenderSystem::sprite(pos.x() - 9, pos.y() - 9, tank[f], tank[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 11, pos.y() - 11, tank[f], tank[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 11, pos.y() - 9, tank[f], tank[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 9, pos.y() - 11, tank[f], tank[0][2], 10, flip);
    }
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
    if (GameVariables::hasBlueprintUnlocked(CounterMotorBP)) s_instance.m_steering.setSteering(&steering_boat_countersteer);

    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});
    ControlStatus controls = Controls::getStatus(true);
    s_instance.m_steering.update(dt, controls.x, controls.y);
    Player::setPosition(SoldierMode, s_instance.m_steering.pos());

    if (!controls.a.held() && (controls.x != 0 || controls.y != 0)) s_instance.m_aim.set(controls.x, controls.y);
    if (Player::weaponCooldown(dt)) Player::s_shot_cooldown += Weapon::checkFireWeapon(controls.a, s_instance.current_weapon, s_instance.m_steering.pos(), s_instance.m_aim, s_instance.m_steering.vel());

    int damage = ProjectileManager::getCollisionDamage(s_instance.m_steering.rect(), bulletMask);
    if (damage > 0 && !s_instance.flashing()) {
        s_instance.flash();
        s_instance.m_health.change(-damage);
        AudioSystem::play(sfxHit2);
        if (!Player::alive(BoatMode)) {
            // TODO: make gameover
            onVehicleDestroyed();
            Player::s_mode = PlayerMode::SoldierMode;
            UI::showHealthbar();
            EffectManager::createExplosionBig(s_instance.m_steering.pos() - Vec2f(6,6));
        }
    }

    static uint16_t disembarkPoints = Helpers::getMask({Terrain::None, Terrain::Mud, Terrain::Grass, Terrain::WaterShallow});
    Vec2f projection = s_instance.m_steering.pos() + s_instance.m_steering.facing() * 12.0f;
    Player::updateCounter();
    if (controls.b.pressed() && mode_switch_counter > 1 && CollisionManager::collides(projection, disembarkPoints)) { // project forward and look for ground

        static uint16_t blocksLanding = Helpers::getMask({Terrain::Wall, Terrain::DestrucableWood, Terrain::DestructableMetal, Terrain::LowWall});
        for (int i = 3; i <= 15; i += 3) {
            projection = s_instance.m_steering.pos() + s_instance.m_steering.facing() * float(i);
            if (CollisionManager::collides(projection, blocksLanding)) {
                i = 18;
            } else if (CollisionManager::collides(projection, disembarkPoints)) {
                projection.setX(std::floor(projection.x() / 6.0f) * 6 + 3);
                projection.setY(std::floor(projection.y() / 6.0f) * 6 + 3);
                Player::setPosition(SoldierMode, projection);
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
    if (!Player::alive(BoatMode)) return;
    Vec2f pos = Camera::worldToScreen(s_instance.m_steering.pos());

    if (Player::s_mode == SoldierMode && Soldier::overlaps(BoatMode)) {
        int f = s_instance.m_steering.rotation_frame();
        bool flip = s_instance.m_steering.facing().x() > 0.1f;
        RenderSystem::sprite(pos.x() - 6, pos.y() - 4, boat[f], boat[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 4, pos.y() - 4, boat[f], boat[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 4, pos.y() - 6, boat[f], boat[0][2], 10, flip);
        RenderSystem::sprite(pos.x() - 6, pos.y() - 6, boat[f], boat[0][2], 10, flip);
    }
    if (s_instance.flashing()) {
        RenderSystem::sprite(pos.x() - 5, pos.y() - 5, boat[s_instance.m_steering.rotation_frame()], boat[0][2], 10, s_instance.m_steering.facing().x() > 0.1f);
    } else {
        RenderSystem::sprite(pos.x() - 5, pos.y() - 5, boat[s_instance.m_steering.rotation_frame()], boat[0][2], s_instance.m_steering.facing().x() > 0.1f);
    }
    Player::drawReticle(BoatMode, s_instance.m_aim);
}


Vehicle &Player::getInstance(PlayerMode mode)
{
    switch (mode) {
    case JeepMode:
        return Jeep::s_instance;
    case TankMode:
        return Tank::s_instance;
    case BoatMode:
        return Boat::s_instance;
    case HelicopterMode:
        return Helicopter::s_instance;
    default:
        return Soldier::s_instance;
    }
}

bool Player::buildVehicleAt(PlayerMode mode, Vec2f position)
{
    bool was_dead = !alive(mode);
    Vehicle &target = getInstance(mode);
    if (target.m_health.value() <= 0 || !Camera::inViewingZone(target.m_steering.pos())) {
        target.m_steering.setPos(position);
    }
    target.m_health.setMax();
    return was_dead;
}

bool Player::hurting()
{
    return Soldier::s_instance.flashing() || Helicopter::s_instance.flashing() || Jeep::s_instance.flashing() || Tank::s_instance.flashing() || Boat::s_instance.flashing();
}

bool Player::alive(PlayerMode m)
{
    Vehicle &target = getInstance(m);
    return target.m_health.value() > 0;
}

bool Player::damaged(PlayerMode m)
{
    Vehicle &target = getInstance(m);
    return target.m_health.value() < target.m_health.max();
}

Statistic &Player::health(PlayerMode m)
{
    return getInstance(m).m_health;
}

void Player::setPosition(PlayerMode m, const Vec2f &pos)
{
    getInstance(m).m_steering.setPos(pos);
}

Vec2f Player::position(PlayerMode m)
{
    return getInstance(m).m_steering.pos();
}

bool Player::available(PlayerMode m)
{
    return GameVariables::hasBlueprintUnlocked(Blueprints(int(m) - 1));
}

Rect Player::bounds(PlayerMode m)
{
    return getInstance(m).m_steering.rect();
}

bool Player::dead()
{
    return Soldier::s_instance.m_health.value() <= 0;
}

Vec2f Player::position()
{
    // TODO: add helicopter offset if in air
    return getInstance(s_mode).m_steering.pos();
}

Rect Player::bounds()
{
    return bounds(s_mode);
}

Weapon::Type Player::currentWeapon()
{
    return getInstance(s_mode).current_weapon;
}

void Player::cycleWeaponNext()
{
    getInstance(s_mode).current_weapon = Weapon::nextWeapon(getInstance(s_mode).current_weapon);
    while (!getInstance(s_mode).current_weapon & s_owned_weapons & getInstance(s_mode).m_valid_weapons) {
        getInstance(s_mode).current_weapon = Weapon::nextWeapon(getInstance(s_mode).current_weapon);
    }
}

void Player::cycleWeaponPrev()
{
    getInstance(s_mode).current_weapon = Weapon::prevWeapon(getInstance(s_mode).current_weapon);
    while (!getInstance(s_mode).current_weapon & s_owned_weapons & getInstance(s_mode).m_valid_weapons) {
        getInstance(s_mode).current_weapon = Weapon::prevWeapon(getInstance(s_mode).current_weapon);
    }
}

WeaponHelper Player::getCurrentWeaponInfo()
{
    return WeaponHelper::getIndexAndMax(getInstance(s_mode).current_weapon, s_owned_weapons & getInstance(s_mode).m_valid_weapons);
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
    Vec2f dir = Player::getInstance(s_mode).m_steering.facing();
    if (s_mode == PlayerMode::SoldierMode) dir = Soldier::s_instance.m_aim;
    const float distance = 17.0f;
    int map_to = dir.getRotationFrame(8);
    static const Vec2f mappings[] = {
        {0, 1},
        {0.383f, 0.924f},
        {0.707f, 0.707f},
        {0.924f, 0.383f},
        {1, 0},
        {0.924f, -0.383f},
        {0.707f, -0.707f},
        {0.383f, -0.924f},
        {0, -1}
    };
    bool flip = dir.x() < 0;
    dir = mappings[8 - map_to];
    if (flip) dir.setX(dir.x() * -1);

    if (s_mode == PlayerMode::JeepMode || s_mode == PlayerMode::TankMode) {
        Vec2f offset = dir.rot90() * 4.0f;
        RenderSystem::incrementColors(49 + dir.x() * distance + offset.x(), 38 + dir.y() * distance + offset.y(), enemy_turret_base[0], 0, dir.x() > 0);
        RenderSystem::incrementColors(49 + dir.x() * distance - offset.x(), 38 + dir.y() * distance - offset.y(), enemy_turret_base[0], 0, dir.x() > 0);
    } else {
        RenderSystem::incrementColors(49 + dir.x() * distance, 38 + dir.y() * distance, enemy_turret_base[0], 0, dir.x() > 0);
    }
}

void Player::updateCounter()
{
    mode_switch_counter++;
}

void Player::storeData()
{
    GameStorage * dat = GameVariables::getData();

    dat->soldierPosition = position(SoldierMode) + Vec2f(0, 6);
    dat->jeepPosition =    position(JeepMode) + (Player::mode() == JeepMode ? Vec2f(0, 6) : Vec2f(0,0));
    dat->boatPosition =    position(BoatMode);
    dat->heliPosition =    position(HelicopterMode);
    dat->tankPosition =    position(TankMode);

    dat->soldierLife =     health(SoldierMode).value();
    dat->jeepLife =        health(JeepMode).value();
    dat->boatLife =        health(BoatMode).value();
    dat->heliLife =        health(HelicopterMode).value();
    dat->tankLife =        health(TankMode).value();
}

void Player::loadData()
{
    GameStorage * dat = GameVariables::getData();

    Soldier::s_instance = Soldier();
    Jeep::s_instance = Jeep();
    Tank::s_instance = Tank();
    Boat::s_instance = Boat();
    Helicopter::s_instance = Helicopter();

    setPosition(SoldierMode, dat->soldierPosition);
    health(SoldierMode).set(dat->soldierLife);
    setPosition(JeepMode, dat->jeepPosition);
    health(JeepMode).set(dat->jeepLife);
    setPosition(TankMode, dat->tankPosition);
    health(TankMode).set(dat->tankLife);
    setPosition(BoatMode, dat->boatPosition);
    health(BoatMode).set(dat->boatLife);
    setPosition(HelicopterMode, dat->heliPosition);
    health(HelicopterMode).set(dat->heliLife);
}

void Player::updateOwnedWeapons()
{
    s_owned_weapons = Weapon::Gun | Weapon::Grenade | Weapon::DualShot | Weapon::Missiles | Weapon::MachineGun;

    // add the rest
//    if (GameVariables::hasBlueprintUnlocked(GeminiGunBP)) s_owned_weapons |= Weapon::DualShot;
}

