//
// Created by ertls on 02.03.2026.
//

#ifndef MIXEDPROJECT_COMMAND_HPP
#define MIXEDPROJECT_COMMAND_HPP
#include <functional>


namespace Engine {
    class Task {
        std::function<void()> _fn_;
    public:
        explicit Task(std::function<void()> fn);
        ~Task() = default;
        void execute() const;
    };

} // Engine

#endif //MIXEDPROJECT_COMMAND_HPP