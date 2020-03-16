#include "combinator.h"

Combinator::Combinator(std::string const& name /* = "Combinator" */) : Mechanic(name) {
}

void Combinator::tick() {

}

std::string Combinator::dump() const {
    return name;
}
