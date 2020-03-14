#include "pump.h"

Pump::Pump() : Mechanic("Pump") {
}

void Pump::tick() {

}

std::string Pump::dump() const {
    return name + "\n";
}
