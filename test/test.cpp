#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <fluid.h>
#include <mechanics/tank.h>
#include <mechanics/pump.h>

TEST_CASE("Fluid movement") {
    FluidVolume vol;
    vol.add("Fluid 1", 10_L);
    REQUIRE(vol.getTotalVolume() == 10_L);

    vol.add("Fluid 2", 10_L);
    REQUIRE(vol.getTotalVolume() == 20_L);
    
    auto s = vol.subtract(10_L);
    REQUIRE(s.getTotalVolume() == 10_L);
    REQUIRE(vol.getTotalVolume() == 10_L);

    auto sv = s.getVolume();
    for (auto v : sv) {
        REQUIRE(v.second == 5_L);
    }

    auto tank = Tank::create("tank1", 99999999_L);
    auto tank2 = Tank::create("tank1", 99999999_L);
    tank->setState(Mechanic::State::Enabled);
    tank2->setState(Mechanic::State::Enabled);
    SECTION("Tank") {
        tank->deposit(vol);
        REQUIRE(tank->getCapacity() == 99999999_L);
        REQUIRE(tank->getQuantity() == 10_L);
    }

    auto pump = Pump::create("pump1");
    pump->setState(Mechanic::State::Enabled);
    pump->setFlow(2000_LpS);
    tank->deposit(FluidVolume("water", 200000_L));
    pump->setSource(tank);
    pump->setDest(tank2);
    auto steps = 1.0 / misc::MechanicsTickRate.to<double>();
    for (auto i = 0; i < steps; ++i) {
        pump->tick();
    }
    REQUIRE(tank2->getQuantity() >= 2000_L * .99);
    REQUIRE(tank2->getQuantity() <= 2000_L * 1.01);
}

TEST_CASE("Fluid Amount") {
    Liters fa(55_L);
    SECTION("Add") {
        REQUIRE(fa + 55_L == 110_L);
    }
}
