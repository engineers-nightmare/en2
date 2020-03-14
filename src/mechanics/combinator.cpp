#include "combinator.h"

Combinator::Combinator() : Mechanic("Combinator") {
}

void Combinator::tick(float dt) {

}

std::string Combinator::dump() const {
    return name + "\n";
}
