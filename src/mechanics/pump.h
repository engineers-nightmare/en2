#pragma once

#include "mechanic.h"

class Pump : public Mechanic {
public:
    Pump();
    virtual void tick() override;
    virtual std::string dump() const override;
};
