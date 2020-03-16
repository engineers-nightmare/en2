#include "torch.h"

Torch::Torch(std::string const& name /* = "Torch" */) : Mechanic(name) {
}

void Torch::tick() {

}

std::string Torch::dump() const {
    return name;
}
