//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/Systems/IO/Keys.hpp"

namespace ggm {
    struct KeyPressEvent {
        Key key;

        friend std::ostream& operator<<(std::ostream& os, const KeyPressEvent& event) {
            return os << toString(event.key);
        }
    };
}
