#include "physics.h"
#include "components.h"

void Physics::update(entt::registry &reg, double t) {
    for (auto [e, pos, vel, con] :
         reg.view<Position, Velocity, Controllable>().each()) {
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);
        vel.rot = con.rot * t * 300;
        vel.x += sx * con.throttle * t;
        vel.y += sy * con.throttle * t;
    }

    for (auto [e, pos, vel] : reg.view<Position, Velocity>().each()) {
        pos.x += vel.x * t;
        pos.y += vel.y * t;
        pos.a += vel.rot * t;
    }
}
