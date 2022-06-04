#include "physics.h"
#include "components.h"
#include <random>

namespace {

auto gen = std::mt19937{std::random_device{}()};

void handleCollisions(entt::registry &reg) {

    auto toRemove = std::vector<entt::entity>{};

    auto vdist = std::normal_distribution(0.f, 2.f);

    auto isCollision = [](Position pos1, Position pos2, float size) {
        auto dx = pos2.x - pos1.x;
        auto dy = pos2.y - pos1.y;

        auto dist2 = dx * dx + dy * dy;
        return dist2 < size * size;
    };

    for (auto [e1, pos1, col] : reg.view<Position, Collidable>().each()) {
        for (auto [e2, pos2, proj] : reg.view<Position, Projectile>().each()) {
            if (proj.damage == 0) {
                continue;
            }

            if (isCollision(pos1, pos2, col.size)) {
                toRemove.push_back(e1);
                toRemove.push_back(e2);
                proj.damage = 0;

                createExplosion(reg, pos1);

                if (col.size > 3.f) {
                    for (size_t i = 0; i < 3; ++i) {
                        auto nx = vdist(gen);
                        auto ny = vdist(gen);
                        createAstroid(reg,
                                      {pos1.x + nx * 2, pos1.y + ny * 2},
                                      {vdist(gen), vdist(gen)},
                                      Collidable{col.size / 2.f});
                    }
                }
            }
        }
    }

    for (auto [e1, pos1, col] : reg.view<Position, Collidable>().each()) {
        for (auto [e2, pos2, proj] :
             reg.view<Position, Controllable>().each()) {
            if (isCollision(pos1, pos2, 5)) {
                toRemove.push_back(e1);
                toRemove.push_back(e2);

                createExplosion(reg, pos1);
                createExplosion(reg, pos2);
            }
        }
    }

    std::sort(toRemove.begin(), toRemove.end());
    auto it = std::unique(toRemove.begin(), toRemove.end());
    toRemove.erase(it, toRemove.end());
    reg.destroy(toRemove.begin(), toRemove.end());
}

} // namespace

void Physics::update(entt::registry &reg, double t) {
    for (auto [e, lifetime] : reg.view<Lifetime>().each()) {
        lifetime.t -= t;
        if (lifetime.t < 0) {
            reg.destroy(e);
        }
    }

    for (auto [e, con, pos, vel] :
         reg.view<Controllable, Position, Velocity>().each()) {
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);
        vel.rot = con.rot / 2.f;

        vel.x += sx * con.throttle * t;
        vel.y += sy * con.throttle * t;
    }

    for (auto [e, con, pos, vel, weap] :
         reg.view<Controllable, Position, Velocity, Weapon>().each()) {
        const float projectileSpeed = 10.;
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);

        if (con.fire) {
            while (weap.currentCooldown > 0) {
                createProjectile(reg,
                                 pos,
                                 {
                                     static_cast<float>(vel.x + sx * 10.),
                                     static_cast<float>(vel.y + sy * 10.),
                                 });
                weap.currentCooldown -= 1;
            }
        }

        weap.currentCooldown += t;
        weap.currentCooldown = std::min(weap.currentCooldown, weap.cooldown);
    }

    for (auto [e, pos, proj] : reg.view<Position, ParticleSmoke>().each()) {
        auto dist = std::normal_distribution<float>{0, 1};
        createParticle(reg, pos, {dist(gen), dist(gen)});
    }

    for (auto [e, pos, vel] : reg.view<Position, Velocity>().each()) {
        pos.x += vel.x * t;
        pos.y += vel.y * t;
        pos.a += vel.rot * t;

        if (pos.x < -10.f) {
            pos.x += _width + 10.f;
        }
        else if (pos.x > _width + 10.f) {
            pos.x -= _width + 10.f;
        }
        if (pos.y < -10.f) {
            pos.y += _height + 10.f;
        }
        else if (pos.y > _width + 10.f) {
            pos.y -= _height + 10.f;
        }
    }

    handleCollisions(reg);
}
