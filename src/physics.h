#pragma once

#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"

struct Physics {
    void update(entt::registry &reg, double t);
    void draw(entt::registry &reg, sdl::Renderer &renderer);
};
