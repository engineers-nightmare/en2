#pragma once

#include <unordered_map>

#include "mechanic.h"
#include "../fluid.h"

class Tank : public Mechanic {
public:
    using ptr = std::shared_ptr<Tank>;
    static ptr create(std::string const& name, Liters cap = 0_L);

protected:
    Liters capacity;
    FluidVolume volume;

public:
    Tank(std::string const& name, Liters cap);
    virtual void tick() override;
    virtual void dump(std::stringstream & stream) const override;

    Liters getQuantity() const;
    Liters getCapacity() const;

    /** get snapshot view of tank contents */
    FluidVolume const& inspect() const;

    /** returns remainder if tried to deposit past capacity */
    FluidVolume deposit(FluidVolume dep);

    /** returns amount withdrawn. Might be different from requested */
    FluidVolume withdraw(Liters w);

    void setCapacity(Liters c);
};
