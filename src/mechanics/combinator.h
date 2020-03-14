#pragma once

#include "mechanic.h"

class Combinator : public Mechanic {
public:
    Combinator();
    virtual void tick() override;
    virtual std::string dump() const override;
};
