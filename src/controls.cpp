#include "controls.h"
#include "components.h"

void Controls::update(entt::registry &reg) {
    for (auto [e, con] : reg.view<Controllable>().each()) {
        con.fire = fire;
        con.rot = -rot;
        con.throttle = throttle;
    }
}
