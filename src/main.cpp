#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

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

    void dumpMechanics() {
        for (auto && mech : mechanics) {
           std::cout << mech->dump() << "\n";
        }
    }
};

class Game {
public:
    Systems systems;

    Game() : systems() {
        // auto tank1 = systems.mechanics.emplace_back(std::make_shared<Tank>("tank1"));
        // auto tank2 = systems.mechanics.emplace_back(std::make_shared<Tank>("tank2"));
        // auto tank3 = systems.mechanics.emplace_back(std::make_shared<Tank>("tank3"));
        // auto tank4 = systems.mechanics.emplace_back(std::make_shared<Tank>("tank4"));
        // auto p1c = systems.mechanics.emplace_back(std::make_shared<Pump>("pump1"));
        // auto p2c = systems.mechanics.emplace_back(std::make_shared<Pump>());
        // auto p3c = systems.mechanics.emplace_back(std::make_shared<Pump>());
        // auto pfc = systems.mechanics.emplace_back(std::make_shared<Pump>());
        // auto pt = systems.mechanics.emplace_back(std::make_shared<Pump>());
        // auto cmb = systems.mechanics.emplace_back(std::make_shared<Combinator>());
        // auto torch = systems.mechanics.emplace_back(std::make_shared<Combinator>());
        // probably want to do these in an order

        auto t1 = std::make_shared<Tank>("tank1");
        auto t2 = std::make_shared<Tank>("tank2");
        auto p1 = std::make_shared<Pump>("pump1");

        systems.mechanics.push_back(t1);
        systems.mechanics.push_back(t2);
        systems.mechanics.push_back(p1);

        t1->setCapacity(100000);
        t1->deposit(100000);
        t2->setCapacity(100000);

        p1->setFlow(8750);

        p1->setSource(t1);
        p1->setDest(t2);
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
                systems.dumpMechanics();
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
