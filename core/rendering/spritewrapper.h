#ifndef SPRITEWRAPPER_H
#define SPRITEWRAPPER_H

#include <cstdint>
#include "game/constants.h"
#include "game/sprites.h"

enum SpriteName {
    BulletSmall     = 0,
    BulletMedium    = 1,
    GrenadeSprite   = 2,

    MissileSprite1  = 3,
    MissileSprite2  = 4,
    MissileSprite3  = 5,
    MissileSprite4  = 6,
    MissileSprite5  = 7,
    MissileSprite6  = 8,
    MissileSprite7  = 9,
    MissileSprite8  = 10,
    MissileSprite9  = 11,

    SmokeSprite     = 12,
    HitSprite       = 13,
    ExplosionSmall  = 14,
    ExplosionBig    = 15,

    POIShopClosed   = 16,
    POIDoorA        = 17,
    POIDoorB        = 18,
    POIDoorC        = 19,
    POIDoorNone     = 20,
    POIShopOpen     = 21,

    DollarSprite    = 22,
    HackingKitSprite= 23,
    Keycard1Sprite  = 24,
    Keycard2Sprite  = 25,
    Keycard3Sprite  = 26,
    BlueprintSprite = 27,

    NoSprite = 50000,
};

struct SpriteConfig {
    const uint8_t * sprite;
    uint8_t frame_count;
    uint8_t counts_per_frame;
    constexpr SpriteConfig(const uint8_t * spr, int fc, float fps) :
        sprite(spr),
        frame_count(fc),
        counts_per_frame(1.0f / (physicsTimestep * fps) + 0.5f)
    {

    }
};

const SpriteConfig sprite_configs[] = {
    { projectile[0], 2, 20.0f },
    { projectile_med[0], 2, 20.0f },
    { projectile_grenade[0], 2, 8.0f },

    { projectile_missile[0], 1, 1.0f },
    { projectile_missile[1], 1, 1.0f },
    { projectile_missile[2], 1, 1.0f },
    { projectile_missile[3], 1, 1.0f },
    { projectile_missile[4], 1, 1.0f },
    { projectile_missile[5], 1, 1.0f },
    { projectile_missile[6], 1, 1.0f },
    { projectile_missile[7], 1, 1.0f },
    { projectile_missile[8], 1, 1.0f },

    { smoke[0], 6, 24.0f },
    { hit[0], 5, 20.0f },
    { explosion_small[0], 7, 14.0f },
    { explosion[0], 7, 18.0f },
    { poi[0], 1, 1},
    { poi[2], 1, 1},
    { poi[3], 1, 1},
    { poi[4], 1, 1},
    { poi[5], 1, 1},
    { poi[1], 1, 1},
    { pickup_dollar[0], 2, 8.0f },
    { pickup_hackingkit[0], 2, 8.0f },
    { pickup_keycard1[0], 2, 8.0f },
    { pickup_keycard2[0], 2, 8.0f },
    { pickup_keycard3[0], 2, 8.0f },
    { pickup_blueprint[0], 2, 8.0f }
};

class SpriteWrapper {
    SpriteName sprite;
    uint16_t counter;

    static uint16_t s_counter;
public:
    SpriteWrapper() {}
    SpriteWrapper(SpriteName n) : sprite(n), counter(s_counter) { }

    static void update();

    int currentFrame() const;

    const uint8_t * data() const;

    const uint16_t countsPerCycle() const;
};

#endif // SPRITEWRAPPER_H
