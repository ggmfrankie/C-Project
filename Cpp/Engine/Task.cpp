//
// Created by ertls on 02.03.2026.
//
//module Engine.Task;
#include "Task.hpp"

namespace Engine {
    void Task::execute() const {
        _fn_();
    }
    Task::Task(std::function<void()> fn): _fn_(std::move(fn)){}
} // Engine