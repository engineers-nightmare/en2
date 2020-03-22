#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>

#include "misc.h"
#include "mechanics/mechanics.h"

class Systems {
public:
    std::vector<std::shared_ptr<Mechanic>> mechanics;

    void tickMechanics() {
        for (auto && mech : mechanics) {
            mech->tick();
        }
    }

    void dumpMechanics(std::stringstream & stream) const {
        for (auto && mech : mechanics) {
           mech->dump(stream);
           stream << "\n";
        }
        stream << "\n";
    }
};

class Game {
private:
    std::stringstream dumpStream;

public:
    Systems systems;

    Game() : systems(), dumpStream() {
        // auto tank1 = systems.mechanics.emplace_back(Tank::create("tank1"));
        // auto tank2 = systems.mechanics.emplace_back(Tank::create("tank2"));
        // auto tank3 = systems.mechanics.emplace_back(Tank::create("tank3"));
        // auto tank4 = systems.mechanics.emplace_back(Tank::create("tank4"));
        // auto p1c = systems.mechanics.emplace_back(Pump::create("pump1"));
        // auto p2c = systems.mechanics.emplace_back(Pump::create());
        // auto p3c = systems.mechanics.emplace_back(Pump::create());
        // auto pfc = systems.mechanics.emplace_back(Pump::create());
        // auto pt = systems.mechanics.emplace_back(Pump::create());
        // auto cmb = systems.mechanics.emplace_back(Combinator::create());
        // auto torch = systems.mechanics.emplace_back(Combinator::create());
        // probably want to do these in an order

        auto t1 = Tank::create("tank1");
        auto t2 = Tank::create("tank2");
        auto t3 = Tank::create("tank3");
        auto p1 = Pump::create("pump1");
        auto p2 = Pump::create("pump2");
        auto p3 = Pump::create("pump3");
        auto c1 = Combinator::create("comb1");

        systems.mechanics.push_back(t1);
        systems.mechanics.push_back(t2);
        systems.mechanics.push_back(t3);
        systems.mechanics.push_back(p1);
        systems.mechanics.push_back(p2);
        systems.mechanics.push_back(p3);
        systems.mechanics.push_back(c1);

        // configure combinator
        {
            c1->setTanks(3);
            c1->setMixRate(10000);
            c1->setMix(0, 10);
            c1->setMix(1, 30);
            c1->setMix(2, 60);
            auto t = c1->getTank(0);
            t->setCapacity(1000);
            t = c1->getTank(1);
            t->setCapacity(1000);
            t = c1->getTank(2);
            t->setCapacity(10000);
            t = c1->getP();
            t->setCapacity(500000);
        }

        //configure tanks
        {
            t1->setCapacity(100000);
            t2->setCapacity(100000);
            t3->setCapacity(100000);
            auto f1 = FluidVolume("Fluid 1", t1->getCapacity());
            auto f2 = FluidVolume("Fluid 2", t2->getCapacity());
            auto f3 = FluidVolume("Fluid 3", t3->getCapacity());
            t1->deposit(f1);
            t2->deposit(f2);
            t3->deposit(f3);
        }

        // configure pumps
        {
            p1->setFlow(87500);
            p2->setFlow(40000);
            p3->setFlow(60000);

            p1->setSource(t1);
            p1->setDest(c1->getTank(0));
            p2->setSource(t2);
            p2->setDest(c1->getTank(1));
            p3->setSource(t3);
            p3->setDest(c1->getTank(2));
        }

    }

    bool init() {
        if (SDL_Init(SDL_INIT_TIMER) < 0) {
            std::cout << "SDL initiaize error: " << SDL_GetError() << "\n";
            return false;
        }

        return true;
    }

    bool run() {
        SDL_Event event;

        auto curTime = SDL_GetTicks() / 1000.0f;
        auto mechAccum = 0.0f;

        while (true) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        return false;
                }
            }

            const auto now = SDL_GetTicks() / 1000.0f;
            auto frameTime = now - curTime;

            if (frameTime > 0.25f) {
                frameTime = 0.25f;
            }
            curTime = now;

            mechAccum += frameTime;

            while (mechAccum >= misc::MechanicsTickRate) {
                // tick mechanics at fixed mechTick rate
                systems.tickMechanics();
                mechAccum -= misc::MechanicsTickRate;
                systems.dumpMechanics(dumpStream);
                std::cout << dumpStream.str();
                dumpStream.str("");
                dumpStream.clear();
            }

            // output state of mechanics
        }

        return true;
    }
};

int main() {
    std::cout << "en2\n";

    Game game;
    if (!game.init()) {
        return 1;
    }

    game.run();

    return 0;
}

void run() {
}
