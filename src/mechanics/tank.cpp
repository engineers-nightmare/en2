#include "tank.h"

Tank::Tank(std::string const &name, Liters cap) : Mechanic(name), capacity(cap) {
}

Tank::ptr Tank::create(std::string const& name, Liters cap /*= 0*/) {
    return std::make_shared<Tank>(name, cap);
}

void Tank::tick() {
    if (state != State::Enabled) {
        return;
    }
}

void Tank::dump(std::stringstream & stream) const {
    auto q = getQuantity();
    stream << name << " |";
    auto pct = q / capacity;
    const auto width = 10;
    for (auto i = 0; i < width; ++i) {
        if (i == (int)(width * pct)) {
            stream << "^";
        }
        else {
            stream << " ";
        }
    }
    stream << "|  " << q.to<double>() << "L / " << capacity.to<double>() << "L ";
    for (auto & fluid : volume.getVolume()) {
        stream << "(" << fluid.first << ": " << fluid.second.to<double>() << "L) ";
    }
}

Liters Tank::getQuantity() const {
    return volume.getTotalVolume();
}

Liters Tank::getCapacity() const {
    return capacity;
}

void Tank::setCapacity(Liters c) {
    capacity = c;
}

FluidVolume const& Tank::inspect() const {
    return volume;
}

FluidVolume Tank::deposit(FluidVolume dep) {
    if (getQuantity() >= capacity) {
        return dep;
    }

    auto over = (getQuantity() + dep.getTotalVolume()) - capacity;

    if (over >= Liters(0)) {
        volume.add(dep.subtract(dep.getTotalVolume() - over));
        return dep;
    }
    else {
        volume.add(dep);
        return FluidVolume();
    }
}

FluidVolume Tank::withdraw(Liters w) {
    return volume.subtract(w);
}
