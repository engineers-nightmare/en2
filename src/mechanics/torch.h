#pragma once

#include "mechanic.h"

class Torch : public Mechanic {
public:
    using ptr = std::shared_ptr<Torch>;

public:
    Torch(std::string const& name = "Torch");
    virtual void tick() override;
    virtual std::string dump() const override;
};
