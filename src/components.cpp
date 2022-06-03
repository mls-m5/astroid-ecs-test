#include "components.h"
#include "drawcomponents.h"
#include <random>

namespace {

auto gen = std::mt19937{std::random_device{}()};

}

void createAstroid(entt::registry &reg,
                   Position pos,
                   Velocity vel,
                   Collidable col) {
    auto entity = reg.create();

    reg.emplace<Position>(entity, pos);
    reg.emplace<Velocity>(entity, vel);
    reg.emplace<Collidable>(entity, col);
    //    reg.emplace<Visible>(entity, Visible{1, drawAstroid});
}

void createPlayer(entt::registry &reg, Position pos) {
    auto entity = reg.create();

    reg.emplace<Position>(entity, pos);
    reg.emplace<Velocity>(entity);
    reg.emplace<Visible>(entity, Visible{1, drawShip});
    reg.emplace<Controllable>(entity);
}

void createProjectile(entt::registry &reg, Position pos, Velocity vel) {

    auto entity = reg.create();

    reg.emplace<Position>(entity, pos);
    reg.emplace<Velocity>(entity, vel);
    reg.emplace<Projectile>(entity);
    reg.emplace<Visible>(entity, Visible{1, drawProjectile});
    reg.emplace<Lifetime>(entity, Lifetime{10.f});
    reg.emplace<ParticleSmoke>(entity, ParticleSmoke{});
}

void createParticle(entt::registry &reg, Position pos, Velocity vel) {
    auto entity = reg.create();

    reg.emplace<Position>(entity, pos);
    reg.emplace<Velocity>(entity, vel);
    reg.emplace<Lifetime>(entity, Lifetime{10.});
    reg.emplace<Point>(entity);
}

void createExplosion(entt::registry &reg, Position pos) {
    for (size_t i = 0; i < 100; ++i) {
        auto entity = reg.create();

        reg.emplace<Position>(entity, pos);
        reg.emplace<Lifetime>(
            entity, Lifetime{std::uniform_real_distribution<float>{2, 4}(gen)});
        reg.emplace<Point>(entity);

        auto dist = std::normal_distribution(0.f, 2.f);
        reg.emplace<Velocity>(entity, dist(gen), dist(gen));
    }
}
