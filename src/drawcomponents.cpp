#include "drawcomponents.h"

void drawAstroid(sdl::RendererView renderer,
                 const Position &pos,
                 const Visible &vis) {
    renderer.drawLine(pos.x, pos.y, pos.x, pos.y + vis.size);
}

void draw(entt::registry &reg, sdl::RendererView renderer) {
    auto view = reg.view<Position, Visible>();

    renderer.drawColor({255, 255, 255, 255});
    for (auto [e, pos, vis] : view.each()) {
        vis.f(renderer, pos, vis);
    }
}
