#include "combinator.h"

Combinator::Combinator() : Mechanic("Combinator") {
}

void Combinator::tick() {

}

std::string Combinator::dump() const {
    return name + "\n";
}
