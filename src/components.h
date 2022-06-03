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
    float size = 10 / 2;
};

// Stuff that collides with collidables
struct Projectile {
    float damage = 1;
};

struct Lifetime {
    float t = 1;
};

struct Controllable {
    int controllNum = 0;
    float rot = 0;
    float throttle = 0;
    bool fire = false;
};

struct Visible {
    float size = 1;
    std::function<void(
        sdl::RendererView renderer, const Position &, const Visible &)>
        f;
};

struct Point {
    float alpha = 1;
};

struct ParticleSmoke {
    float interval = .1;
    float nextT = 0;
};

void createAstroid(entt::registry &,
                   Position pos,
                   Velocity vel = {},
                   Collidable = {});

void createPlayer(entt::registry &, Position pos);

void createProjectile(entt::registry &, Position pos, Velocity vel);

void createParticle(entt::registry &reg, Position pos, Velocity vel);

void createExplosion(entt::registry &, Position pos);
