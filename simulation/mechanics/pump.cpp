#include "pump.h"

Pump::Pump(std::string const& name)
: Mechanic(name), source(nullptr), dest(nullptr), flow(0.0f), holding(), cycle(Cycle::Take) {
}

Pump::ptr Pump::create(std::string const& name /*= "Pump"*/) {
    return std::make_shared<Pump>(name);
}

void Pump::tick() {
    if (state != State::Enabled) {
        return;
    }

    switch (cycle) {
        case Cycle::Take:
            take();
            cycle = Cycle::Put;
            break;
        case Cycle::Put:
            put();
            cycle = Cycle::Take;
            break;
    }
}

void Pump::dump(std::stringstream & stream) const {
    stream << name << " (" << flow.to<double>() << "L) ";
    holding.dump(stream);
}

void Pump::setSource(Tank::ptr const& s) {
    source = s;
}

void Pump::setDest(Tank::ptr const& d) {
    dest = d;
}

void Pump::setFlow(LitersPerSecond f) {
    flow = f;
}

Tank::ptr const& Pump::getSource() const {
    return source;
}

Tank::ptr const& Pump::getDest() const {
    return dest;
}

FluidVolume const& Pump::getHolding() const {
    return holding;
}

LitersPerSecond const& Pump::getFlow() const {
    return flow;
}

void Pump::take() {
    if (!source) {
        return;
    }

    Liters f = flow * 2.0 * misc::MechanicsTickRate;
    auto td = misc::MechanicsTickRate.to<double>();
    auto fd = flow.to<double>();
    auto over = f - holding.getTotalVolume();
    if (over > 0_L) {
        holding.add(source->withdraw(over));
    }
}

void Pump::put() {
    if (!dest) {
        return;
    }

    holding = dest->deposit(holding);    
}
