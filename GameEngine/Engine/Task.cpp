//
// Created by ertls on 02.03.2026.
//
//module ggm.Task;
#include "Task.hpp"

namespace ggm {
    void Task::execute() const {
        mFunction();
    }
    Task::Task(std::function<void()> fn): mFunction(std::move(fn)){}
} // ggm