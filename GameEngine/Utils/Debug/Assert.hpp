//
// Created by Stefan on 24.10.2026.
//

#pragma once
#include <concepts>
#include <iostream>
#include <source_location>

#include "Message.hpp"

namespace ggm::Assert {
    [[noreturn]] void fail(const std::source_location& loc, std::string_view msg);

    void openGlLoaded(const std::source_location& loc = std::source_location::current());

    template<std::predicate<> Callable>
    void run(Callable callable, Message&& message = {}, const std::source_location& loc = std::source_location::current()) {
        if (callable()) return;
        fail(loc, std::move(message.data));
    }

    template<std::convertible_to<bool> Expression>
    requires (!std::predicate<Expression>)
    void run(Expression expression, Message&& message = {}, const std::source_location& loc = std::source_location::current()) {
        if (expression) return;
        fail(loc, std::move(message.data));
    }
}
