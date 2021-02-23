#ifndef HELPERS_H
#define HELPERS_H

#include <cstdint>
#include <initializer_list>

namespace Helpers {
    static uint16_t getMask(std::initializer_list<uint8_t> collisions) {
        uint16_t mask = 0;
        for(auto c : collisions) {
            mask |= (1 << c);
        }
        return mask;
    }
};

#endif // HELPERS_H
