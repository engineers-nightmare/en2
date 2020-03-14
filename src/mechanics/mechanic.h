#pragma once

#include <string>

class Mechanic {
protected:
    std::string name;

public:
    Mechanic(std::string const& name) : name(name) {}

    virtual void tick() = 0;

    // return a string of things done since last tick
    virtual std::string dump() const = 0;
};

