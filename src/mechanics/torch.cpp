#include "torch.h"

Torch::Torch() : Mechanic("Torch") {
}

void Torch::tick() {

}

std::string Torch::dump() const {
    return name + "\n";
}
