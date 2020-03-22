#pragma once

#include "mechanic.h"

class Torch : public Mechanic {
public:
    using ptr = std::shared_ptr<Torch>;
    static ptr create(std::string const& name = "Torch");

public:
    Torch(std::string const& name = "Torch");
    virtual void tick() override;
    virtual void dump(std::stringstream & stream) const override;
};
