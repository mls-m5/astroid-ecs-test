#pragma once

#include "entt/entity/registry.hpp"
#include "sdlpp/render.hpp"
#include <functional>

struct Position {
    float x = 0;
    float y = 0;
    float a = 0;

    Position operator-(Position other) const {
        return {x - other.x, y - other.y};
    }

    float abs2() const {
        return x * x + y * y;
    }

    float abs() const {
        return std::sqrt(abs2());
    }

    Position operator*(float value) const {
        return {x * value, y * value};
    }

    Position operator+(Position other) const {
        return {x + other.x, y + other.y};
    }
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
    bool shouldExplode = false;
};

struct Controllable {
    int controllNum = 0;
    float rot = 0;
    float throttle = 0;
    bool fire = false;
    bool secondary = false;
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

struct Weapon {
    float cooldown = .1;
    float currentCooldown = cooldown;
};

struct SecondaryWeapon {
    int num = 10;
    int type = 0;
};

struct Dead {
    char dummy = 0;
};

struct Homing {
    entt::entity target;
    Position pos;
    Velocity direction;
    float timeLeft = 1;
    float speed = .1;
};

void createAstroid(entt::registry &,
                   Position pos,
                   Velocity vel = {},
                   Collidable = {});

void createPlayer(entt::registry &, Position pos);

void createProjectile(entt::registry &, Position pos, Velocity vel);

void createParticle(entt::registry &, Position pos, Velocity vel);

void createExplosion(entt::registry &, Position pos);

void setDead(entt::registry &, entt::entity);

void createHomingMissiles(entt::registry &, Position pos, Velocity vel);
