#include "tank.h"

Tank::Tank() : Mechanic("Tank") {
}

void Tank::tick(float dt) {

}

std::string Tank::dump() const {
    return name + "\n";
}
