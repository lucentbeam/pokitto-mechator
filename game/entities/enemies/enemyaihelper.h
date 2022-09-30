#ifndef ENEMYAIHELPER_H
#define ENEMYAIHELPER_H

#include <cstdint>

#include "core/utilities/vec.h"

enum AIMode : uint8_t {
    Walking,
    Preparing
};

namespace EnemyAIHelper
{

bool updateEntity(Vec2f pos, Vec2f &aim, uint16_t &action_counter, AIMode &mode, int8_t &life, uint8_t &damage_frames, uint16_t walkmask, uint16_t bulletmask, bool checkcollisions, bool drops, bool &shooting);

}

#endif // ENEMYAIHELPER_H
