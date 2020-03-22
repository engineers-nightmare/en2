#include "pump.h"

Pump::Pump(std::string const& name)
: Mechanic(name), source(nullptr), dest(nullptr), flow(0.0f), holding() {
}

Pump::ptr Pump::create(std::string const& name /*= "Pump"*/) {
    return std::make_shared<Pump>(name);
}

void Pump::tick() {
    if (state != State::Enabled) {
        return;
    }

    take();
    put();
}

void Pump::dump(std::stringstream & stream) const {
    stream << name << "  ";
    holding.dump(stream);
}

void Pump::setSource(Tank::ptr const& s) {
    source = s;
}

void Pump::setDest(Tank::ptr const& d) {
    dest = d;
}

void Pump::setFlow(int64_t f) {
    flow = f / (1.0f / misc::MechanicsTickRate);
}

void Pump::take() {
    if (!source) {
        return;
    }

    auto over = flow - holding.getTotalVolume();
    if (over > 0) {
        holding.add(source->withdraw(over));
    }
}

void Pump::put() {
    if (!dest) {
        return;
    }

    holding = dest->deposit(holding);    
}
