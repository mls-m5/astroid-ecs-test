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

void Physics::draw(entt::registry &reg, sdl::Renderer &renderer) {
    auto view = reg.view<Position, Visible>();

    renderer.drawColor({255, 255, 255, 255});
    for (auto [e, pos, vis] : view.each()) {
        renderer.drawLine(pos.x, pos.y, pos.x, pos.y + vis.size);
    }
}
