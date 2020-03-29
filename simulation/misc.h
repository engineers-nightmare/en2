#pragma once

#define DISABLE_PREDEFINED_UNITS
#define ENABLE_PREDEFINED_TIME_UNITS
#define ENABLE_PREDEFINED_LENGTH_UNITS
#define ENABLE_PREDEFINED_VOLUME_UNITS
#include <units.h>

using namespace units::literals;

namespace misc {
    constexpr units::time::second_t MechanicsTickRate = 1_s / 60.0;
}

