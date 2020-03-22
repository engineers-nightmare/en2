#include "torch.h"

Torch::Torch(std::string const& name) : Mechanic(name) {
}

Torch::ptr Torch::create(std::string const& name /*= "Torch"*/) {
    return std::make_shared<Torch>(name);
}

void Torch::tick() {

}

void Torch::dump(std::stringstream & stream) const {
    stream << name;
}
