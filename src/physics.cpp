#include "physics.h"
#include "components.h"

void Physics::update(entt::registry &reg) {
    auto view = reg.view<Position, Velocity>();

    for (auto [e, pos, vel] : view.each()) {
        pos.x += vel.x;
        pos.y += vel.y;
        pos.a += vel.rot;
    }
}

void Physics::render(entt::registry &reg, sdl::Renderer &renderer) {
    auto view = reg.view<Position, Visible>();

    renderer.drawColor({255, 255, 255, 255});
    for (auto [e, pos, vis] : view.each()) {
        renderer.drawLine(pos.x, pos.y, pos.x, pos.y + vis.size);
    }
}
