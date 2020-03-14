#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include "mechanics/mechanics.h"

class Systems {
public:
    std::vector<std::shared_ptr<Mechanic>> mechanics;

    void tickMechanics(float dt) {
        for (auto && mech : mechanics) {
            mech->tick(dt);
        }
    }

    void dumpMechanics() {
        for (auto && mech : mechanics) {
           std::cout << mech->dump();
        }
    }
};

class Game {
public:
    Systems systems;

    Game() : systems() {
        auto tank1 = systems.mechanics.emplace_back(std::make_shared<Tank>());
        auto tank2 = systems.mechanics.emplace_back(std::make_shared<Tank>());
        auto tank3 = systems.mechanics.emplace_back(std::make_shared<Tank>());
        auto tank4 = systems.mechanics.emplace_back(std::make_shared<Tank>());
        auto p1c = systems.mechanics.emplace_back(std::make_shared<Pump>());
        auto p2c = systems.mechanics.emplace_back(std::make_shared<Pump>());
        auto p3c = systems.mechanics.emplace_back(std::make_shared<Pump>());
        auto pfc = systems.mechanics.emplace_back(std::make_shared<Pump>());
        auto pt = systems.mechanics.emplace_back(std::make_shared<Pump>());
        auto cmb = systems.mechanics.emplace_back(std::make_shared<Combinator>());
        auto torch = systems.mechanics.emplace_back(std::make_shared<Combinator>());

        // probably want to do these in an order
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
        const auto mechTick = 1.0f / 60.0f;

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

            while (mechAccum >= mechTick) {
                // tick mechanics at fixed mechTick rate
                systems.tickMechanics(mechTick);
                mechAccum -= mechTick;
            }

            // output state of mechanics
            systems.dumpMechanics();
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
