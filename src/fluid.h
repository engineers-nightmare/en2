#pragma once

#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "misc.h"

typedef std::string FluidType;

typedef units::volume::liter_t Liters;
using LitersPerSecond = decltype(1_L / 1_s);

LitersPerSecond operator""_LpS(long double lpt);

LitersPerSecond operator""_LpS(unsigned long long int lpt);

class FluidVolume {
public:
    typedef std::unordered_map<FluidType, Liters> Volume;

    Volume volume;

    /**
     * return vector of fluid type and ratio of
     * that fluid in this volume
     */
    std::unordered_map<FluidType, double> getRatios() const;

public:
    FluidVolume();
    FluidVolume(FluidType type, Liters amount);

    void dump(std::stringstream & stream) const;

    Liters getTotalVolume() const;
    Volume const& getVolume() const;

    void add(FluidType type, Liters amount);
    void add(FluidVolume vol);

    /**
     * returns proportional mix of all fluids in this volume
     * equal to or less than amount
     * Will be less than if there isn't enough in this volume
     * to satisfy the request
     */
    FluidVolume subtract(Liters amount);
};
