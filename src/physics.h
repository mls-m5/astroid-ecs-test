#pragma once

#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"

struct Physics {
    void update(entt::registry &reg);
    void render(entt::registry &reg, sdl::Renderer &renderer);
};
