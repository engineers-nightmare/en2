#pragma once

#include <string>
#include <memory>

#include "../misc.h"

class Mechanic {
public:
    using ptr = std::shared_ptr<Mechanic>;

protected:
    std::string name;

public:
    Mechanic(std::string const& name) : name(name) {}

    virtual void tick() = 0;

    // return a string of things done since last tick
    virtual std::string dump() const = 0;
};

