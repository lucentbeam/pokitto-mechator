#include "player.h"
#include "game/ui/ui.h"

Vehicle Player::s_soldier(8,8*6, 15*6, 20.0f, 1.0f, {Terrain::Wall, Terrain::WaterDeep, Terrain::DestrucableWood, Terrain::DestructableMetal}, 4, 4);

Vehicle Player::s_jeep(12, 26*6, 8*6, 50.0f, 0.1f, {Terrain::Wall, Terrain::WaterDeep, Terrain::WaterShallow, Terrain::DestrucableWood, Terrain::DestructableMetal}, 9, 9, 0.05f);

PlayerMode Player::s_mode = PlayerMode::Soldier;


Vehicle::Vehicle(int8_t hp, float x, float y, float speed, float cornering, std::initializer_list<uint8_t> collisions, float w, float h, float friction) :
    steering(x, y, speed, cornering, collisions, w, h, friction),
    health(hp)
{

}

void Player::update(float dt) {
    static uint16_t bulletMask = Helpers::getMask({Targets::PlayerTarget, Targets::GroundTarget});

    ControlStatus controls = Controls::getStatus(true);
    int damage = 0;
    if (s_mode == PlayerMode::Soldier) {
        s_soldier.steering.update(dt, controls.x , controls.y);
        s_jeep.steering.update(dt, 0.0f, 0.0f);
        damage = ProjectileManager::getCollisionDamage(s_soldier.steering.rect(), bulletMask);
        if (damage > 0) {
            s_soldier.health.change(-damage);
        }
    } else {
        s_jeep.steering.update(dt, controls.x, controls.y);
        damage = ProjectileManager::getCollisionDamage(s_jeep.steering.rect(), bulletMask);
        if (damage > 0) {
            s_jeep.health.change(-damage);
        }
        if (s_jeep.steering.moving()) {
            Terrain current = CollisionManager::getTerrainAt(s_jeep.steering.pos().x(), s_jeep.steering.pos().y());
            s_jeep.shake.intensity = current == Terrain::Grass ? Rumbler::Vigorous : Rumbler::Slight;
            s_jeep.steering.scaleMaxSpeed(current == Terrain::Grass ? 0.5f : 1.0f);
            s_jeep.shake.update();
        }
    }
    if (controls.c.releasedWithin(60)) {
        if (s_mode == PlayerMode::Soldier) {
            Vec2f dp = s_jeep.steering.pos() - s_soldier.steering.pos();
            if (dp.length() < 6) {
                s_mode = PlayerMode::Jeep;
                UI::showHealthbar();
            }
        } else {
            s_soldier.steering.copyPosition(s_jeep.steering);
            s_mode = PlayerMode::Soldier;
            UI::showHealthbar();
        }
    }
    if (s_mode == PlayerMode::Soldier) {
        const float shots_per_second = 3.0f;
        float frames_per_second = 1.0f / dt;
        float frames_per_shot = frames_per_second / shots_per_second;
        if (controls.a.downEvery(1, int(frames_per_shot))) {
            Projectile * p = ProjectileManager::create(s_soldier.steering.pos(), s_soldier.steering.aim() * 100.0f, 3, 0.5f)
                ->setSprite({projectile[0]}, 20)
                ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
        }
    } else {
        const float shots_per_second = 2.0f;
        float frames_per_second = 1.0f / dt;
        float frames_per_shot = frames_per_second / shots_per_second;
        if (controls.a.downEvery(1, int(frames_per_shot))) {
            Vec2f offset = s_jeep.steering.aim().rot90();
            ProjectileManager::create(s_jeep.steering.pos() + offset * 4.0f, s_jeep.steering.aim() * 150.0f, 3, 0.35f)
                    ->setSprite({projectile[0]}, 20)
                    ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
            ProjectileManager::create(s_jeep.steering.pos() - offset * 4.0f, s_jeep.steering.aim() * 150.0f, 3, 0.35f)
                    ->setSprite({projectile[0]}, 20)
                    ->setTargetMask({EnemyTarget, GroundTarget, AirTarget});
        }
        if (controls.b.pressed()) {

            ProjectileManager::create(s_jeep.steering.pos(), s_jeep.steering.vel() * 0.85f + s_jeep.steering.facing() * 50.0f, 4, 0.5f)
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
    Vec2f jpos = Camera::worldToScreen(s_jeep.steering.pos());
    RenderSystem::sprite(jpos.x() - 7, jpos.y() - 7 - s_jeep.shake.offset(1), jeep[s_jeep.steering.rotation_frame()], jeep[0][2], s_jeep.steering.facing().x() > 0);

    if (s_mode == PlayerMode::Soldier) {
        // soldier rendering
        uint8_t sprite = s_soldier.steering.facing().y() < 0 ? 3 : 0;
        static int counter = 0;
        if (s_soldier.steering.moving()) {
            counter++;
            const int increment = 6;
            int mod = counter % (increment * 4);
            sprite += mod < increment ? 0 : mod < increment * 2 ? 1 : mod < increment * 3 ? 2 : 1;
        } else {
            sprite += 1;
        }
        Vec2f spos = Camera::worldToScreen(s_soldier.steering.pos());
        RenderSystem::sprite(spos.x()- 3, spos.y() - 3, soldier[sprite], soldier[0][2], s_soldier.steering.facing().x() > 0);
    }


}

Vec2f Player::position()
{
    switch (s_mode) {
    case PlayerMode::Jeep:
        return s_jeep.steering.pos();
        break;
    default:
        return s_soldier.steering.pos();
    }
}
