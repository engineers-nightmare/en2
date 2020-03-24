#pragma once

#include <unordered_map>

#include "mechanic.h"
#include "tank.h"

class Torch : public Mechanic {
public:
    using ptr = std::shared_ptr<Torch>;
    static ptr create(std::string const& name = "Torch");

    enum class TorchState {
        Inactive,
        Suboptimal,
        Optimal,
    };

protected:
    Tank::ptr tankFeed;

    FluidAmount burnRate;
    std::unordered_map<FluidType, uint> combustibles;
    float nearTolerance;
    float farTolerance;
    FluidVolume holding;

    TorchState torchState;

public:
    Torch(std::string const& name = "Torch");
    virtual void tick() override;
    virtual void dump(std::stringstream & stream) const override;

    /** in μl/s */
    void setBurnRate(FluidAmount burn);

    /**
     * set tolerances to burn rate discrepancies
     * allows behavior on within, just out, well past
     * if any 1 ratio is <= near -> within
     * ratio <= far && ratio > near -> just out
     * ratio > far -> well past
     */
    void setTolerance(float near, float far);

    /**
     * set combustible and ratio
     * set to 0 to remove
     */
    void setCombustible(FluidType const& fluid, uint percent);

    TorchState const& getTorchState() const;

    Tank::ptr const& getFeedTank() const;
};
