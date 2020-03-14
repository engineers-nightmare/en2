#pragma once

#include "mechanic.h"

class Torch : public Mechanic {
public:
    Torch();
    virtual void tick(float dt) override;
    virtual std::string dump() const override;
};
