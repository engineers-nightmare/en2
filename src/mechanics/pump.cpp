#include "pump.h"

Pump::Pump(std::string const& name /*= "Pump"*/)
: Mechanic(name), source(nullptr), dest(nullptr), flow(0.0f), holding(0.0f) {
}

void Pump::tick() {
    take();
    put();
}

std::string Pump::dump() const {
    return name + "  " + std::to_string(holding);
}

void Pump::setSource(Tank::ptr const& s) {
    source = s;
}

void Pump::setDest(Tank::ptr const& d) {
    dest = d;
}

void Pump::setFlow(int64_t f) {
    flow = f * misc::MechanicsTickRate;
}

void Pump::take() {
    auto over = flow - holding;
    if (over > 0) {
        holding += source->withdraw(over);
    }
}

void Pump::put() {
    holding = dest->deposit(holding);    
}
