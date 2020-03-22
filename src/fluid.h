#pragma once

#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::string FluidType;

typedef int64_t FluidAmount;

class FluidVolume {
public:
    typedef std::unordered_map<FluidType, FluidAmount> Volume;

    Volume volume;

    /**
     * return vector of fluid type and ratio of
     * that fluid in this volume
     */
    std::unordered_map<FluidType, float> getRatios() const;

public:
    FluidVolume();
    FluidVolume(FluidType type, int64_t amount);

    void dump(std::stringstream & stream) const;

    int64_t getTotalVolume() const;
    Volume const& getVolume() const;

    void add(FluidType type, int64_t amount);
    void add(FluidVolume const& vol);

    /**
     * returns proportional mix of all fluids in this volume
     * equal to or less than amount
     * Will be less than if there isn't enough in this volume
     * to satisfy the request
     */
    FluidVolume subtract(int64_t amount);
};
