//
// Created by Stefan on 20.10.2026.
//

#pragma once
namespace ggm {
    struct KeyPressEvent {
        int keycode;

        friend std::ostream& operator<<(std::ostream& os, const KeyPressEvent& event) {
            return os << event.keycode;
        }
    };
}
