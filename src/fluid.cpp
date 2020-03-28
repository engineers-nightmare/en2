#include "fluid.h"


LitersPerSecond operator""_LpS(long double lps) {
    return LitersPerSecond(lps);
}

LitersPerSecond operator""_LpS(unsigned long long int lps) {
    return LitersPerSecond(lps);
}


FluidVolume::FluidVolume() : volume() {}

FluidVolume::FluidVolume(FluidType type, Liters amount) {
    add(type, amount);
}

void FluidVolume::dump(std::stringstream & stream) const {
    for (auto const& f: volume) {
        stream << f.first << "(" << f.second.to<double>() << "L) ";
    }
}

std::unordered_map<FluidType, double> FluidVolume::getRatios() const {
    std::unordered_map<FluidType, double> ratios;
    auto tv = getTotalVolume();
    for (auto const& f: volume) {
        if (tv == Liters(0.0)) {
            ratios[f.first] = 0;
            continue;
        }
        ratios[f.first] = f.second.to<double>() / tv.to<double>();
    }
    return ratios;
}

Liters FluidVolume::getTotalVolume() const {
    Liters v(0);
    for (auto const& f: volume) {
        v += f.second;
    }
    return v;
}


FluidVolume::Volume const& FluidVolume::getVolume() const {
    return volume;
}

void FluidVolume::add(FluidType type, Liters amount) {
    volume[type] += amount;
}

void FluidVolume::add(FluidVolume vol) {
    for (auto && fv : vol.volume) {
        volume[fv.first] += fv.second;
    }
}

FluidVolume FluidVolume::subtract(Liters amount) {
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
