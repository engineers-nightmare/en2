#pragma once

#include "mechanic.h"

class Tank : public Mechanic {
public:
    Tank();
    virtual void tick() override;
    virtual std::string dump() const override;
};
