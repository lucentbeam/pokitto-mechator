#include "blinker.h"

#include "game/constants.h"

Blinker::Blinker(float tot, float on) : total(tot), on_length(on) {}

void Blinker::update() {
    counter += physicsTimestep;
    while (counter >= total) counter -= total;
}

bool Blinker::active() const { return counter > (total - on_length); }
