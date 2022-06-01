#pragma once

#include "entt/entity/registry.hpp"

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
};

void createAstroid(entt::registry &, Position pos);
