#include "physics.h"
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/window.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    auto window = sdl::Window{"game", 0, 0, 300, 300, SDL_WINDOW_SHOWN};
    auto renderer = sdl::Renderer{window, -1, SDL_RENDERER_ACCELERATED};

    auto physics = Physics{};

    auto registry = entt::registry{};

    for (bool running = true; running;) {
        for (auto o = sdl::pollEvent(); o; o = sdl::pollEvent()) {
            auto event = *o;

            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        physics.update(registry);

        renderer.drawColor({255, 255, 255, 255});
        renderer.drawLine(0, 0, 100, 100);

        renderer.present();
    }

    return 0;
}
