#include "components.h"

void createAstroid(entt::registry &reg) {
    auto entity = reg.create();

    reg.emplace<Position>(entity);
    reg.emplace<Velocity>(entity);
    reg.emplace<Collidable>(entity);
    reg.emplace<Visible>(entity);
}
