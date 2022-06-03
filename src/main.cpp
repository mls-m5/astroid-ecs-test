#include "components.h"
#include "controls.h"
#include "drawcomponents.h"
#include "physics.h"
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/window.hpp"
#include <chrono>
#include <random>

void handleControls(Controls &controls, const sdl::Event &event, bool state) {
    auto keyEvent = event.key;

    static auto map = std::vector{
        std::pair{SDL_SCANCODE_LEFT, Controls::Left},
        std::pair{SDL_SCANCODE_RIGHT, Controls::Right},
        std::pair{SDL_SCANCODE_UP, Controls::Up},
        std::pair{SDL_SCANCODE_DOWN, Controls::Down},
        std::pair{SDL_SCANCODE_SPACE, Controls::Fire},
    };

    for (auto &c : map) {
        if (c.first == keyEvent.keysym.scancode) {
            controls.setKey(c.second, state);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    auto width = 300;
    auto height = 300;
    auto window = sdl::Window{"game", 0, 0, 300, 300, SDL_WINDOW_SHOWN};
    auto renderer = sdl::Renderer{
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

    auto registry = entt::registry{};
    auto physics = Physics{
        registry, static_cast<float>(width), static_cast<float>(height)};
    auto controls = Controls{};

    auto gen = std::mt19937{std::random_device{}()};
    auto xdist = std::uniform_real_distribution(0.f, static_cast<float>(width));
    auto ydist =
        std::uniform_real_distribution(0.f, static_cast<float>(height));

    auto dist = std::normal_distribution(0.f, 2.f);
    auto linear = std::uniform_real_distribution(0.f, 3.14f);
    for (size_t i = 0; i < 10; ++i) {
        createAstroid(registry,
                      Position{xdist(gen), ydist(gen), linear(gen)},
                      {dist(gen), dist(gen), dist(gen) / 10.f},
                      {10.f});
    }

    createPlayer(registry, {width / 2.f, height / 2.f});

    using namespace std::chrono;
    auto lastFrameTime = high_resolution_clock::now();

    for (bool running = true; running;) {
        for (auto o = sdl::pollEvent(); o; o = sdl::pollEvent()) {
            auto event = *o;

            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                handleControls(controls, event, true);
                break;
            case SDL_KEYUP:
                handleControls(controls, event, false);
                break;
            }
        }

        auto frameTime = high_resolution_clock::now();

        auto t =
            duration_cast<microseconds>(frameTime - lastFrameTime).count() /
            100'000.;
        lastFrameTime = frameTime;

        controls.update(registry);
        physics.update(registry, t);

        renderer.drawColor({0, 0, 0, 255});
        renderer.fillRect();

        draw(registry, renderer);

        renderer.present();
    }

    return 0;
}
