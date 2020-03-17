#include "tank.h"

Tank::Tank(std::string const &name) : Mechanic(name) {
}

Tank::ptr Tank::create(std::string const& name /*= "Tank"*/) {
    return std::make_shared<Tank>(name);
}

void Tank::tick() {
}

std::string Tank::dump() const {
    std::string d = name + " |";
    auto pct = (float)quantity / (float)capacity;
    const auto width = 10;
    for (auto i = 0; i < width; ++i) {
        if (i == (int64_t)(width * pct)) {
            d += "^";
        }
        else {
            d += " ";
        }
    }
    d += "|  " + std::to_string(quantity) + "/" + std::to_string(capacity);
    return d;
}

int64_t Tank::getQuantity() const {
    return quantity;
}

int64_t Tank::getCapacity() const {
    return capacity;
}

void Tank::setCapacity(int64_t c) {
    capacity = c;
}

int64_t Tank::deposit(int64_t d) {
    if (quantity >= capacity) {
        return d;
    }

    auto over = (quantity + d) - capacity;

    if (over >= 0) {
        quantity = capacity;
        return over;
    }
    else {
        quantity += d;
        return 0;
    }
}

int64_t Tank::withdraw(int64_t w) {
    auto amt = std::min(w, quantity);
    quantity -= amt;
    return amt;
}
