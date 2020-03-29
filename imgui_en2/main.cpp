#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>

#include <fluid.h>
#include <misc.h>
#include <mechanics/mechanics.h>

#include "imgui/imgui.h"
#include "imgui/imnodes.h"
#include "imgui_sdl.h"
#include "imgui/imgui_impl_sdl.h"


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
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

public:
    Systems systems;

    Game() : systems(), dumpStream() {
        dumpStream << std::setiosflags(std::ios::fixed) << std::setprecision(5);

        initMechanics();
    }

    void initMechanics() {
        systems.mechanics.clear();

        auto t1 = Tank::create("tank1"); t1->setState(Mechanic::State::Disabled);
        auto t2 = Tank::create("tank2"); t2->setState(Mechanic::State::Disabled);
        auto t3 = Tank::create("tank3"); t3->setState(Mechanic::State::Disabled);
        auto p1 = Pump::create("pump1"); p1->setState(Mechanic::State::Disabled);
        auto p2 = Pump::create("pump2"); p2->setState(Mechanic::State::Disabled);
        auto p3 = Pump::create("pump3"); p3->setState(Mechanic::State::Disabled);
        auto p4 = Pump::create("pump4"); p4->setState(Mechanic::State::Disabled);
        auto c1 = Combinator::create("comb1"); c1->setState(Mechanic::State::Disabled);
        auto to = Torch::create("torch"); to->setState(Mechanic::State::Disabled);

        systems.mechanics.push_back(t1);
        systems.mechanics.push_back(t2);
        systems.mechanics.push_back(t3);
        systems.mechanics.push_back(p1);
        systems.mechanics.push_back(p2);
        systems.mechanics.push_back(p3);
        systems.mechanics.push_back(p4);
        systems.mechanics.push_back(c1);
        systems.mechanics.push_back(to);

        // configure combinator
        {
            c1->setState(Mechanic::State::Enabled);
            c1->setTanks(3);
            c1->setMixRate(1.168_LpS * 0.75);
            c1->setMix(0, 10);
            c1->setMix(1, 30);
            c1->setMix(2, 60);
            auto t = c1->getTank(0);
            t->setCapacity(100_L);
            t = c1->getTank(1);
            t->setCapacity(100_L);
            t = c1->getTank(2);
            t->setCapacity(100_L);
            t = c1->getProductTank();
            t->setCapacity(100_L);
        }

        //configure tanks
        {
            t1->setCapacity(100_L);
            t2->setCapacity(100_L);
            t3->setCapacity(100_L);
            auto f1 = FluidVolume("Fluid 1", t1->getCapacity());
            auto f2 = FluidVolume("Fluid 2", t2->getCapacity());
            auto f3 = FluidVolume("Fluid 3", t3->getCapacity());
            t1->deposit(f1);
            t2->deposit(f2);
            t3->deposit(f3);
            to->getFeedTank()->setCapacity(1_L);
        }

        // configure torch
        {
            to->setTolerance(5, 10);
            to->setBurnRate(0.168_LpS);
            to->setCombustible("Fluid 1", 10);
            to->setCombustible("Fluid 2", 30);
            to->setCombustible("Fluid 3", 60);
        }

        // configure pumps
        {
            p1->setFlow(1.875_LpS);
            p2->setFlow(1.500_LpS);
            p3->setFlow(2.900_LpS);
            p4->setFlow(11_LpS);

            p1->setSource(t1);
            p1->setDest(c1->getTank(0));
            p2->setSource(t2);
            p2->setDest(c1->getTank(1));
            p3->setSource(t3);
            p3->setDest(c1->getTank(2));
            p4->setSource(c1->getProductTank());
            p4->setDest(to->getFeedTank());
        }
    }

    bool init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "SDL initiaize error: " << SDL_GetError() << "\n";
            return false;
        }

        window = SDL_CreateWindow("EN2 IMGUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        ImGui::CreateContext();
        ImGuiSDL::Initialize(renderer, 1920, 1080);
        imnodes::Initialize();

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
        SDL_SetRenderTarget(renderer, texture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, nullptr);

        return true;
    }

    bool run() {
        SDL_Event event;

        auto curTime = SDL_GetTicks() / 1000.0f;
        auto mechAccum = 0.0f;

        bool mechsEnabled = false;

        bool run = true;
        while (run) {
            ImGuiIO& io = ImGui::GetIO();
            int wheel = 0;

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        run = false;
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                        {
                            io.DisplaySize.x = static_cast<float>(event.window.data1);
                            io.DisplaySize.y = static_cast<float>(event.window.data2);
                        }
                        break;
                    case SDL_MOUSEWHEEL:
                        wheel = event.wheel.y;
                        break;
                }
            }

            const auto now = SDL_GetTicks() / 1000.0f;
            auto frameTime = now - curTime;

            if (frameTime > 0.25f) {
                frameTime = 0.25f;
            }
            curTime = now;

            mechAccum += frameTime;

            constexpr auto mechTick = misc::MechanicsTickRate.to<float>();
            while (mechAccum >= mechTick) {
                // tick mechanics at fixed mechTick rate
                systems.tickMechanics();
                mechAccum -= mechTick;
                systems.dumpMechanics(dumpStream);
                std::cout << dumpStream.str();
                dumpStream.str("");
                dumpStream.clear();
            }

            // output state of mechanics

            int mouseX, mouseY;
            const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

            // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
            
            io.DeltaTime = 1.0f / 60.0f;
            io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
            io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
            io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
            io.MouseWheel = static_cast<float>(wheel);

            ImGui::NewFrame();

            // ImGui::ShowDemoWindow();

            ImGui::Begin("Controls");
            if (ImGui::Checkbox("Mechanics enabled", &mechsEnabled)) {
                for (auto const& m : systems.mechanics) {
                    if (mechsEnabled) {
                        m->setState(Mechanic::State::Enabled);
                    }
                    else {
                        m->setState(Mechanic::State::Disabled);
                    }
                }
            }
            if (ImGui::Button("Restart")) {
                initMechanics();
                mechsEnabled = false;
            }
            ImGui::End();

            ImGui::Begin("Mechanics");
            imnodes::BeginNodeEditor();
            ImVec2 tankPos(50, 50);
            int tankWidth = 200;
            int tankYAdjust = 150;
            ImVec2 pumpPos(tankPos.x + tankWidth + 50, 50);
            int pumpYAdjust = 150;
            int pumpWidth = 200;
            ImVec2 combPos(pumpPos.x + pumpWidth + 50, 50);
            int combYAdjust = 150;
            int combWidth = 200;
            ImVec2 torchPos(combPos.x + combWidth + 50, 50);
            int torchYAdjust = 150;
            int histWidth = 50;

            for (auto const& m : systems.mechanics) {
                std::string name = std::string{m->getName()};
                auto id = std::hash<std::string>{}(name);
                if (auto tank = std::dynamic_pointer_cast<Tank>(m)) {
                    imnodes::BeginNode(id, tankPos);
                    tankPos.y += tankYAdjust;
                    {
                        imnodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted((std::string("Tank: " + name).c_str()));
                        imnodes::EndNodeTitleBar();
                    }

                    float arr[] = { (tank->getQuantity() / tank->getCapacity()) };
                    ImGui::PushItemWidth(histWidth);
                    ImGui::PlotHistogram("", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));
                    imnodes::BeginOutputAttribute(id);
                    imnodes::EndAttribute();
                }
                else if (auto pump = std::dynamic_pointer_cast<Pump>(m)) {
                    imnodes::BeginNode(id, pumpPos);
                    pumpPos.y += pumpYAdjust;
                    {
                        imnodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted((std::string("Pump: " + name).c_str()));
                        imnodes::EndNodeTitleBar();
                    }

                    float arr[] = { (pump->getHolding().getTotalVolume() / (pump->getFlow() / misc::MechanicsTickRate)).to<float>() };
                    ImGui::PushItemWidth(histWidth);
                    ImGui::PlotHistogram("", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));

                    imnodes::BeginInputAttribute(id + 1);
                    imnodes::EndAttribute();

                    imnodes::BeginOutputAttribute(id + 2);
                    imnodes::EndAttribute();
                }
                else if (auto comb = std::dynamic_pointer_cast<Combinator>(m)) {
                    imnodes::BeginNode(id, combPos);
                    combPos.y += combYAdjust;
                    {
                        imnodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted((std::string("Combinator: " + name).c_str()));
                        imnodes::EndNodeTitleBar();
                    }

                    for (auto const& t : comb->getTanks()) {
                        auto d = std::hash<std::string>{}(t->getName());
                        imnodes::BeginInputAttribute(d);
                        ImGui::TextUnformatted(t->getName().c_str());
                        ImGui::PushItemWidth(100);
                        ImGui::ProgressBar(t->getQuantity() / t->getCapacity(), ImVec2(0.0f,0.0f));
                        imnodes::EndAttribute();
                    }

                    auto t = comb->getProductTank();
                    auto d = std::hash<std::string>{}(t->getName());
                    imnodes::BeginOutputAttribute(d);
                    ImGui::TextUnformatted(t->getName().c_str());
                    ImGui::PushItemWidth(100);
                    ImGui::ProgressBar(t->getQuantity() / t->getCapacity(), ImVec2(0.0f,0.0f));
                    imnodes::EndAttribute();
                }
                else if (auto torch = std::dynamic_pointer_cast<Torch>(m)) {
                    imnodes::BeginNode(id, torchPos);
                    torchPos.y += torchYAdjust;
                    {
                        imnodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted((std::string("Torch: " + name).c_str()));
                        imnodes::EndNodeTitleBar();
                    }

                    auto t = torch->getFeedTank();
                    auto d = std::hash<std::string>{}(t->getName());
                    imnodes::BeginInputAttribute(d);
                    ImGui::PushItemWidth(100);
                    ImGui::TextUnformatted(t->getName().c_str());
                    ImGui::ProgressBar(t->getQuantity() / t->getCapacity(), ImVec2(0.0f,0.0f));
                    imnodes::EndAttribute();
                }
                imnodes::EndNode();
            }

            for (auto const& m : systems.mechanics) {
                if (auto pump = std::dynamic_pointer_cast<Pump>(m)) {

                    auto id = std::hash<std::string>{}(pump->getName());
                    auto src = std::hash<std::string>{}(pump->getSource()->getName());
                    auto dest = std::hash<std::string>{}(pump->getDest()->getName());

                    imnodes::Link(id + 1, id + 1, src);
                    imnodes::Link(id + 2, id + 2, dest);
                }
            }

            imnodes::EndNodeEditor();
            ImGui::End();

            SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
            SDL_RenderClear(renderer);

            ImGui::Render();
            ImGuiSDL::Render(ImGui::GetDrawData());

            SDL_RenderPresent(renderer);
        }

        imnodes::SaveCurrentEditorStateToIniFile("editor.ini");

        ImGuiSDL::Deinitialize();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        imnodes::Shutdown();
        ImGui::DestroyContext();

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
