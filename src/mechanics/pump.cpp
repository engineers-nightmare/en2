#include "pump.h"

Pump::Pump() : Mechanic("Pump") {
}

void Pump::tick(float dt) {

}

std::string Pump::dump() const {
    return name + "\n";
}
