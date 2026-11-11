//
// Created by Stefan on 11.11.2026.
//

#pragma once
#include <format>
namespace ggm {
    struct Message {
        template<typename... Args>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Message(std::format_string<Args...> fmt, Args&&... args) :
            data(std::format(fmt, std::forward<Args>(args)...))
        {}

        Message() = default;

        std::string data;
    };
}
