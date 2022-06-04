#include "components.h"
#include "controls.h"
#include "drawcomponents.h"
#include "em.h"
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
        std::pair{SDL_SCANCODE_RETURN, Controls::Secondary},
    };

    for (auto &c : map) {
        if (c.first == keyEvent.keysym.scancode) {
            controls.setKey(c.second, state);
            break;
        }
    }
}

const auto width = 640;
const auto height = 480;

void startLevel(entt::registry &registry) {
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
}

using namespace std::chrono;

struct App {

    sdl::Window window{"game", 0, 0, width, height, SDL_WINDOW_SHOWN};
    sdl::Renderer renderer{
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

    entt::registry registry{};
    Physics physics{
        registry, static_cast<float>(width), static_cast<float>(height)};
    Controls controls{};

    high_resolution_clock::time_point lastFrameTime =
        high_resolution_clock::now();

    bool running = true;

    App() {
        startLevel(registry);

        createPlayer(registry, {width / 2.f, height / 2.f});
    }

    void update() {
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
};

int main(int argc, char *argv[]) {
    static auto app = App{};

#ifdef __EMSCRIPTEN__
    auto updateFrame = +[]() { app.update(); };
    emscripten_set_main_loop(updateFrame, 0, 1);
#else
    for (; app.running;) {
        app.update();
    }
#endif

    return 0;
}
