#include "controls.h"
#include "components.h"

void Controls::setKey(Key key, bool state) {
    states.at(key) = state;

    throttle = states.at(Up) - states.at(Down);
    rot = states.at(Right) - states.at(Left);
    fire = states.at(Fire);
    secondary = states.at(Secondary);
}

void Controls::update(entt::registry &reg) const {
    for (auto [e, con] : reg.view<Controllable>().each()) {
        con.fire = fire;
        con.secondary = secondary;
        con.rot = -rot;
        con.throttle = throttle;
    }
}
