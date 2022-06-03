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

    void setKey(Key key, bool state);

    std::array<int, Count> states;

    float rot = 0;
    float throttle = 0;
    bool fire = false;

    void update(entt::registry &reg) const;
};
