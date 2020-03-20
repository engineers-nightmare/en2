#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <fluid.h>
#include <mechanics/tank.h>
#include <mechanics/pump.h>

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

    auto tank = Tank::create("tank1", 123456789);
    auto tank2 = Tank::create("tank1", 123456789);
    tank->deposit(vol);
    SECTION("Tank") {
        REQUIRE(tank->getCapacity() == 123456789);
        REQUIRE(tank->getQuantity() == 10);
    }
}
