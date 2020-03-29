#include <algorithm>
#include <assert.h>
#include <optional>

#include "../misc.h"
#include "combinator.h"

Combinator::Combinator(std::string const& name)
: Mechanic(name), tankProduct(Tank::create("Product")),
  tanks(), mixes(), mixRatios(), mixRate(0) {
}

Combinator::ptr Combinator::create(std::string const& name /*= "Combinator"*/) {
    return std::make_shared<Combinator>(name);
}

void Combinator::tick() {
    if (state != State::Enabled) {
        return;
    }

    std::vector<Liters> candidates(tanks.size());

    for (size_t i = 0; i < tanks.size(); ++i) {
        auto mix = mixes[i];
        auto tank = tanks[i];
        auto ratio = mixRatios[i];
        auto req = tank->getQuantity() / ratio;
        candidates[i] = req;
    }

    std::vector<units::volume::liter> tables(tanks.size());
    for (size_t c = 0; c < candidates.size(); ++c) {
        auto candidate = candidates[c];
        for (size_t i = 0; i < tanks.size(); ++i) {
            auto tank = tanks[i];
            auto ratio = mixRatios[i];
            auto m = ratio * candidate;
            if (m > tank->getQuantity()) {
                candidates[c] = 0_L;
                break;
            }
        }
    }

    std::optional<size_t> candidateIndex = std::nullopt;
    for (size_t c = 0; c < candidates.size(); ++c) {
        if (candidates[c] != 0_L) {
            if (candidateIndex.has_value()) {
                candidateIndex = candidates[c] > candidates[candidateIndex.value()] ? c : candidateIndex.value();
            }
            else {
                candidateIndex = c;
            }
        }
    }

    if (!candidateIndex.has_value()) {
        return;
    }

    Liters candidate = std::min(tankProduct->getCapacity() - tankProduct->getQuantity(), candidates[candidateIndex.value()]);
    Liters mr = mixRate * misc::MechanicsTickRate;
    candidate = std::min(mr, candidate);

    for (size_t i = 0; i < candidates.size(); ++i) {
        auto & tank = tanks[i];
        auto r = mixRatios[i];
        auto p = r * candidate;
        auto v = tank->withdraw(p);
        tankProduct->deposit(v);
    }

    for (auto & tank : tanks) {
        tank->tick();
    }
    tankProduct->tick();
}

void Combinator::dump(std::stringstream & stream) const {
    stream << name << " ratios: ";
    for (auto const &m : mixRatios) {
        stream << m << ":";
    }
    stream << "\n";
    for (auto & tank : tanks) {
        tank->dump(stream);
        stream << "\n|";
    }
    tankProduct->dump(stream);
}

void Combinator::setTanks(uint num) {
    tanks  = std::vector<Tank::ptr>(num, nullptr);
    mixes = std::vector<uint>(num, 0);
    mixRatios = std::vector<double>(num, 0);
    for (auto i = 0; i < num; ++i) {
        tanks[i] = Tank::create(name + "_tank" + std::to_string(i));
    }
}

Tank::ptr const& Combinator::getProductTank() const {
    return tankProduct;
}

Tank::ptr const& Combinator::getTank(size_t index) const {
    return tanks[index];
}

std::vector<Tank::ptr> const& Combinator::getTanks() const {
    return tanks;
}

void Combinator::setMix(size_t tank, uint amount) {
    assert(tank < tanks.size());
    amount = std::clamp(amount, 0u, 100u);
    mixes[tank] = amount;

    sumMixes = 0;
    for (auto const& m : mixes) {
        sumMixes += m;
    }

    for (auto i = 0; i < tanks.size(); ++i) {
        mixRatios[i] = (float)mixes[i] / sumMixes;
    }
}

void Combinator::setMixRate(LitersPerSecond m) {
    mixRate = m;
}
