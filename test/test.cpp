#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <fluid.h>
#include <mechanics/tank.h>

TEST_CASE("Fluid movement") {
    FluidVolume vol;
    vol.add("Fluid 1", 10);
    REQUIRE(vol.getTotalVolume() == 10);

    vol.add("Fluid 2", 10);
    REQUIRE(vol.getTotalVolume() == 20);
    
    auto s = vol.subtract(10);
    REQUIRE(s.getTotalVolume() == 10);
    REQUIRE(vol.getTotalVolume() == 10);

    auto sv = s.getVolume();
    for (auto v : sv) {
        REQUIRE(v.second == 5);
    }

    SECTION("Tank") {
        auto tank = Tank::create("tank1", 123456789);
        REQUIRE(tank->getCapacity() == 123456789);
        tank->deposit(vol);
        REQUIRE(tank->getQuantity() == 10);
    }
}

