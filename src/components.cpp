#include "components.h"
#include "drawcomponents.h"

void createAstroid(entt::registry &reg, Position pos, Velocity vel) {
    auto entity = reg.create();

    reg.emplace<Position>(entity, pos);
    reg.emplace<Velocity>(entity, vel);
    reg.emplace<Collidable>(entity);
    reg.emplace<Visible>(entity, Visible{1, drawAstroid});
}
