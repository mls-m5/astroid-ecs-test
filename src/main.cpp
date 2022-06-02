#include "components.h"
#include "physics.h"
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/window.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    auto window = sdl::Window{"game", 0, 0, 300, 300, SDL_WINDOW_SHOWN};
    auto renderer = sdl::Renderer{window, -1, SDL_RENDERER_ACCELERATED};

    auto physics = Physics{};
    auto registry = entt::registry{};

    createAstroid(registry, {20, 20}, {1, 1});

    using namespace std::chrono;
    auto lastFrameTime = high_resolution_clock::now();

    for (bool running = true; running;) {
        for (auto o = sdl::pollEvent(); o; o = sdl::pollEvent()) {
            auto event = *o;

            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        auto frameTime = high_resolution_clock::now();

        auto t =
            duration_cast<microseconds>(frameTime - lastFrameTime).count() /
            100'000.;
        lastFrameTime = frameTime;

        physics.update(registry, t);

        renderer.drawColor({0, 0, 0, 255});
        renderer.fillRect();
        physics.draw(registry, renderer);

        renderer.present();
    }

    return 0;
}
