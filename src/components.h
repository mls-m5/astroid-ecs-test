#pragma once

#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"
#include <functional>

struct Position {
    float x = 0;
    float y = 0;
    float a = 0;
};

struct Velocity {
    float x = 0;
    float y = 0;
    float rot = 0;
};

struct Collidable {
    float size = 1;
};

struct Visible {
    float size = 1;
    std::function<void(
        sdl::RendererView renderer, const Position &, const Visible &)>
        f;
};

void createAstroid(entt::registry &, Position pos, Velocity vel = {});

void createPlayer(entt::registry &, Position pos);
