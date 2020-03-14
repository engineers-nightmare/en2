#include "tank.h"

Tank::Tank() : Mechanic("Tank") {
}

void Tank::tick() {

}

std::string Tank::dump() const {
    return name + "\n";
}
