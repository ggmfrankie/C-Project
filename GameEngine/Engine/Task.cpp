//
// Created by ertls on 02.03.2026.
//
//module Engine.Task;
#include "Task.hpp"

namespace Engine {
    void Task::execute() const {
        mFunction();
    }
    Task::Task(std::function<void()> fn): mFunction(std::move(fn)){}
} // Engine