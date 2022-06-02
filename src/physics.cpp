#include "physics.h"
#include "components.h"

void Physics::update(entt::registry &reg, double t) {
    auto view = reg.view<Position, Velocity>();

    for (auto [e, pos, vel] : view.each()) {
        pos.x += vel.x * t;
        pos.y += vel.y * t;
        pos.a += vel.rot * t;
    }
}
