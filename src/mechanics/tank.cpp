#include "tank.h"

Tank::Tank(std::string const &name, int64_t cap) : Mechanic(name), capacity(cap) {
}

Tank::ptr Tank::create(std::string const& name, int64_t cap /*= 0*/) {
    return std::make_shared<Tank>(name, cap);
}

void Tank::tick() {
}

std::string Tank::dump() const {
    auto q = getQuantity();
    std::string d = name + " |";
    auto pct = (float)q / (float)capacity;
    const auto width = 10;
    for (auto i = 0; i < width; ++i) {
        if (i == (int64_t)(width * pct)) {
            d += "^";
        }
        else {
            d += " ";
        }
    }
    d += "|  " + std::to_string(q) + "/" + std::to_string(capacity) + " ";
    for (auto & fluid : volume.getVolume()) {
        d += "(" + fluid.first + ": " + std::to_string(fluid.second) + ") ";
    }
    
    return d;
}

int64_t Tank::getQuantity() const {
    return volume.getTotalVolume();
}

int64_t Tank::getCapacity() const {
    return capacity;
}

void Tank::setCapacity(int64_t c) {
    capacity = c;
}

FluidVolume Tank::deposit(FluidVolume dep) {
    if (getQuantity() >= capacity) {
        return dep;
    }

    auto over = (getQuantity() + dep.getTotalVolume()) - capacity;

    if (over >= 0) {
        volume.add(dep.subtract(dep.getTotalVolume() - over));
        return dep;
    }
    else {
        volume.add(dep);
        return FluidVolume();
    }
}

FluidVolume Tank::withdraw(int64_t w) {
    return volume.subtract(w);
}
