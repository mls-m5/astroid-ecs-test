#pragma once

#include "entt/entity/registry.hpp"
#include <array>

struct Controls {
    enum Key {
        Left,
        Right,
        Up,
        Down,
        Fire,

        Count,
    };

    void setKey(Key key, bool state) {
        states.at(key) = state;

        throttle = states.at(Up) - states.at(Down);
        rot = states.at(Right) - states.at(Left);
        fire = states.at(Fire);
    }

    std::array<int, Count> states;

    float rot = 0;
    float throttle = 0;
    bool fire = false;

    void update(entt::registry &reg);
};
