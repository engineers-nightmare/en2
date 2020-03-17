#pragma once

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
    int64_t flow;
    int64_t holding;

public:
    Pump(std::string const& name);
    virtual void tick() override;
    virtual std::string dump() const override;

    void setSource(Tank::ptr const& s);
    void setDest(Tank::ptr const& d);

    /** set flow rate in ml/s */
    void setFlow(int64_t f);
    void take();
    void put();
};
