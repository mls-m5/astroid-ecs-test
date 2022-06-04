#pragma once

#include "components.h"
#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"

struct Position;
struct Collidable;

inline const auto width = 640;
inline const auto height = 480;

struct Game {
    Game(entt::registry &reg, float width, float height);

    void update(entt::registry &reg, double t);

    static void onPlayerDeath(entt::registry &, entt::entity);

    void astroidCreated(entt::registry &, entt::entity);
    void astroidDestroyed(entt::registry &, entt::entity);

    void startLevel(entt::registry &registry);

private:
    float _width = 0;
    float _height = 0;

    int numAstroids = 0;
};
