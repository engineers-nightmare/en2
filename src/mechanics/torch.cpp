#include "torch.h"

Torch::Torch() : Mechanic("Torch") {
}

void Torch::tick(float dt) {

}

std::string Torch::dump() const {
    return name + "\n";
}
