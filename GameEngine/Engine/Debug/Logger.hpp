//
// Created by Stefan on 22.03.2026.
//

#pragma once
#include <format>
#include <source_location>

#include "../EngineDefines.hpp"


namespace ggm::Logger {
    enum class LogLevel {
        NO_WARNINGS = -1,
        NONE,
        INFO,
        DEBUG,
        TRACE
    };

    struct Message {
        template<typename... Args>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Message(std::format_string<Args...> fmt, Args&&... args) :
            data(std::format(fmt, std::forward<Args>(args)...))
        {}
        std::string data;
    };

    void setLevel(LogLevel level = LogLevel::INFO);

    void log(std::string_view level, const Message& message, const std::source_location& sourceLocation);

    void info(const Message& message, const std::source_location& loc = std::source_location::current());
    void debug(const Message& message, const std::source_location& loc = std::source_location::current());
    void trace(const Message& message, const std::source_location& loc = std::source_location::current());

    void warning(const Message& message, const std::source_location& loc = std::source_location::current());

    void error(const Message& message, const std::source_location& loc = std::source_location::current());
}