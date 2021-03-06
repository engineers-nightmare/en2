#pragma once

#include "../fluid.h"
#include "mechanic.h"
#include "tank.h"

/**
 *  A pump's flow rate is also its internal capacity
 * Flow rate should be expressed in terms of 'per tick rate'
 * Each tick a pump should first withdraw up to flow rate from `source`
 * then it should try to deposit into `dest`
 * If `dest` deposit returns anything, that stays/goes back into the internal
 * "tank"
 * 
 * flow rate is milliliters per tick duration
 */

class Pump : public Mechanic {
public:
    using ptr = std::shared_ptr<Pump>;
    static ptr create(std::string const& name = "Pump");

protected:
    Tank::ptr source;
    Tank::ptr dest;
    LitersPerSecond flow;
    FluidVolume holding;

    enum class Cycle {
        Take,
        Put,
    } cycle;

public:
    Pump(std::string const& name);
    virtual void tick() override;
    virtual void dump(std::stringstream & stream) const override;

    void setSource(Tank::ptr const& s);
    void setDest(Tank::ptr const& d);

    void setFlow(LitersPerSecond f);

    Tank::ptr const& getSource() const;
    Tank::ptr const& getDest() const;

    FluidVolume const& getHolding() const;

    LitersPerSecond const& getFlow() const;

    void take();
    void put();
};
