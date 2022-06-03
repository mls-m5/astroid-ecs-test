#pragma once

#include "components.h"
#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"

struct Position;
struct Collidable;

struct Physics {
    Physics(entt::registry &reg, float width, float height)
        : _width{width}
        , _height{height} {}

    void update(entt::registry &reg, double t);

private:
    float _width = 0;
    float _height = 0;
};
