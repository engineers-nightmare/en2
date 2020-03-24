#pragma once

#include <unordered_map>

#include "mechanic.h"
#include "../fluid.h"

class Tank : public Mechanic {
public:
    using ptr = std::shared_ptr<Tank>;
    static ptr create(std::string const& name, int64_t cap = 0);

protected:
    int64_t capacity;
    FluidVolume volume;

public:
    Tank(std::string const& name, int64_t cap);
    virtual void tick() override;
    virtual void dump(std::stringstream & stream) const override;

    int64_t getQuantity() const;
    int64_t getCapacity() const;

    /** get snapshot view of tank contents */
    FluidVolume const& inspect() const;

    /** returns remainder if tried to deposit past capacity */
    FluidVolume deposit(FluidVolume dep);

    /** returns amount withdrawn. Might be different from requested */
    FluidVolume withdraw(int64_t w);

    void setCapacity(int64_t c);
};
