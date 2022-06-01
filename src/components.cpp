#include "components.h"

void createAstroid(entt::registry &reg, Position pos) {
    auto entity = reg.create();

    reg.emplace<Position>(entity, pos);
    reg.emplace<Velocity>(entity);
    reg.emplace<Collidable>(entity);
    reg.emplace<Visible>(entity);
}
