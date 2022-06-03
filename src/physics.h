#pragma once

#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"

struct Physics {
    Physics(float width, float height)
        : _width{width}
        , _height{height} {}

    void update(entt::registry &reg, double t);

private:
    float _width = 0;
    float _height = 0;
};
