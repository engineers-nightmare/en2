#pragma once

#include "mechanic.h"

class Tank : public Mechanic {
public:
    using ptr = std::shared_ptr<Tank>;

protected:
    int64_t capacity;
    int64_t quantity;

public:
    Tank(std::string const& name = "Tank");
    virtual void tick() override;
    virtual std::string dump() const override;

    int64_t getQuantity() const;
    int64_t getCapacity() const;

    /** returns remainder if tried to deposit past capacity */
    int64_t deposit(int64_t d);

    /** returns amount withdrawn. Might be different from requested */
    int64_t withdraw(int64_t w);

    void setCapacity(int64_t c);
};
