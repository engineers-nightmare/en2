#include "fluid.h"

FluidVolume::FluidVolume() : volume() {}

FluidVolume::FluidVolume(FluidType type, int64_t amount) {
    add(type, amount);
}

void FluidVolume::dump(std::stringstream & stream) const {
    for (auto const& f: volume) {
        stream << f.first << "(" << f.second << ") ";
    }
}

std::unordered_map<FluidType, float> FluidVolume::getRatios() const {
    std::unordered_map<FluidType, float> ratios;
    auto tv = getTotalVolume();
    for (auto const& f: volume) {
        if (tv == 0) {
            ratios[f.first] = 0;
            continue;
        }
        ratios[f.first] = (float)f.second / (float)tv;
    }
    return ratios;
}

int64_t FluidVolume::getTotalVolume() const {
    int64_t v = 0;
    for (auto const& f: volume) {
        v += f.second;
    }
    return v;
}


FluidVolume::Volume const& FluidVolume::getVolume() const {
    return volume;
}

void FluidVolume::add(FluidType type, int64_t amount) {
    volume[type] += amount;
}

void FluidVolume::add(FluidVolume const& vol) {
    for (auto && fv : vol.volume) {
        volume[fv.first] += fv.second;
    }
}

FluidVolume FluidVolume::subtract(int64_t amount) {
    auto t = getTotalVolume();
    amount = std::min(t, amount);
    auto ratios = getRatios();
    FluidVolume v;
    
    for (auto & f: volume) {
        auto const& ft = f.first;
        auto & fv = f.second;
        auto r = ratios[ft] * amount;
        v.add(ft, r);
        fv -= r;
    }

    return v;
}
