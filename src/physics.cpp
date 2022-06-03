#include "physics.h"
#include "components.h"
#include <random>

namespace {

auto gen = std::mt19937{std::random_device{}()};

}

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
        vel.rot = con.rot;

        vel.x += sx * con.throttle * t;
        vel.y += sy * con.throttle * t;

        const float projectileSpeed = 10.;

        if (con.fire) {
            createProjectile(reg,
                             pos,
                             {
                                 static_cast<float>(vel.x + sx * 10.),
                                 static_cast<float>(vel.y + sy * 10.),
                             });
        }
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

    auto toRemove = std::vector<entt::entity>{};

    for (auto [e1, pos1, col] : reg.view<Position, Collidable>().each()) {
        for (auto [e2, pos2, proj] : reg.view<Position, Projectile>().each()) {
            if (proj.damage == 0) {
                continue;
            }

            auto dx = pos2.x - pos1.x;
            auto dy = pos2.y - pos1.y;

            auto dist2 = dx * dx + dy * dy;
            if (dist2 < col.size * col.size) {
                toRemove.push_back(e1);
                toRemove.push_back(e2);
                proj.damage = 0;

                createExplosion(reg, pos1);
            }
        }
    }

    std::sort(toRemove.begin(), toRemove.end());
    auto it = std::unique(toRemove.begin(), toRemove.end());
    toRemove.erase(it, toRemove.end());
    reg.destroy(toRemove.begin(), toRemove.end()); // Somtimes causes segfault
}
