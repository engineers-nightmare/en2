#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "../misc.h"

class Mechanic {
public:
    using ptr = std::shared_ptr<Mechanic>;
    static ptr create(std::string const& name = "Mechanic");

    enum class State {
        Disabled,
        Enabled,
    };

protected:
    std::string name;
    State state;

public:
    Mechanic(std::string const& name) : name(name), state(State::Enabled) {}

    void setState(State state) { this->state = state; }

    Mechanic::State getState() const { return state; }

    std::string const& getName() const { return name; }

    virtual void tick() = 0;

    // return a string of things done since last tick
    virtual void dump(std::stringstream & stream) const = 0;
};

