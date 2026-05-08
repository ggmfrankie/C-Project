//
// Created by ertls on 02.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include <functional>

namespace Engine {
    class Task {
        std::function<void()> _fn_;
    public:
        explicit Task(std::function<void()> fn);
        ~Task() = default;
        void execute() const;
    };

}
