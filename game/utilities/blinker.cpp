#include "blinker.h"

#include "game/constants.h"

Blinker::Blinker(float tot, float on) : total(tot), on_length(on) {}

void Blinker::update() {
    counter += physicsTimestep;
    while (counter >= total) counter -= total;
}

bool Blinker::active(float offset) const {
    float val = counter + offset;
    while (val >= total) val -= total;
    return val > (total - on_length);
}
