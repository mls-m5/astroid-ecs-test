#include "physics.h"
#include "components.h"

void Physics::update(entt::registry &reg, double t) {
    for (auto [e, lifetime] : reg.view<Lifetime>().each()) {
        lifetime.t -= t;
        if (lifetime.t < 0) {
            reg.destroy(e);
        }
    }

    for (auto [e, con, pos, vel] :
         reg.group<Controllable, Position, Velocity>().each()) {
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);
        vel.rot = con.rot;

        vel.x += sx * con.throttle * t;
        vel.y += sy * con.throttle * t;

        const float projectileSpeed = 10.;

        if (con.fire) {
            createProjectile(reg,
                             pos,
                             {static_cast<float>(vel.x + sx * 10.),
                              static_cast<float>(vel.y + sy * 10.)},
                             {});
        }
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
}
