#include <algorithm>

#include "torch.h"

Torch::Torch(std::string const& name)
: Mechanic(name), tankFeed(Tank::create(name + "_feed")),
  burnRate(0), combustibles(),
  nearTolerance(0), farTolerance(0), holding(),
  torchState(TorchState::Inactive) {
}

Torch::ptr Torch::create(std::string const& name /*= "Torch"*/) {
    return std::make_shared<Torch>(name);
}

void Torch::tick() {
    if (state != State::Enabled) {
        return;
    }

    holding.add(tankFeed->withdraw(burnRate));
    if (holding.getTotalVolume() < burnRate) {
        torchState = TorchState::Inactive;
        return;
    }

    auto ratios = holding.getRatios();
    std::unordered_map<FluidType, float> thresh;
    bool combustiblesPresent = true;
    // is each combustible represented in the feed tank
    for (auto & c : combustibles) {
        if (ratios.end() == ratios.find(c.first)) {
            combustiblesPresent = false;
            break;
        }
    }
    // without all combustibles present, torch can't possibly burn

    // even with too many fluids in a tank the ratios for combustion
    // might still be met
    auto poorDueToExtraComponents = false;
    if (combustiblesPresent && ratios.size() > combustibles.size()) {
        // poor performance
        poorDueToExtraComponents = true;
    }

    auto within = 0;
    auto justOut = 0;
    auto wellPast = 0;
    for (auto & ratio : ratios) {
        auto c = combustibles[ratio.first];
        auto th = std::abs(((float)c / 100.0f - ratio.second)) * 100.0;
        if (th <= nearTolerance) {
            // this one is in optimal tolerance
            within++;
        }
        else if (th > nearTolerance && th <= farTolerance) {
            // this one is just past tolerance
            justOut++;
        }
        else if (th > farTolerance) {
            // this one is outside of tolerance
            wellPast++;
        }
    }

    if (wellPast) {
        // torch can't work
        torchState = TorchState::Inactive;
    }
    else if (justOut) {
        // torch is performing suboptimally
        torchState = TorchState::Suboptimal;
    }
    else if (within) {
        if (poorDueToExtraComponents) {
            // torch is performing suboptimally
            // because of too many components in mix
            torchState = TorchState::Suboptimal;
        }
        else {
            // torch is performing optimally
            torchState = TorchState::Optimal;
        }
    }
    holding.subtract(burnRate);
}

void Torch::dump(std::stringstream & stream) const {
    std::string stateOut = "";
    switch (torchState) {
        case TorchState::Inactive:
            stateOut = "|< -----";
            break;
        case TorchState::Suboptimal:
            stateOut = "|< =====";
            break;
        case TorchState::Optimal:
            stateOut = "|< <<<<<";
            break;
    }
    stream << name << " " << stateOut << "\n\\";
    tankFeed->dump(stream);
}

void Torch::setBurnRate(FluidAmount burn) {
    burnRate = burn / (1.0f / misc::MechanicsTickRate);
}

void Torch::setTolerance(float near, float far) {
    nearTolerance = near;
    farTolerance = far;
}

void Torch::setCombustible(FluidType const& fluid, uint percent) {
    percent = std::clamp(percent, 0u, 100u);
    if (percent == 0) {
        combustibles.erase(fluid);
    }
    else {
        combustibles[fluid] = percent;
    }
}

Torch::TorchState const& Torch::getTorchState() const {
    return torchState;
}

Tank::ptr const& Torch::getFeedTank() const {
    return tankFeed;
}
