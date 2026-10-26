//
// Created by Stefan on 22.03.2026.
//
#include "../EngineDefines.hpp"
#include "Logger.hpp"

#include <filesystem>
#include <iostream>

namespace ggm::Logger {
    static auto gLogLevel = LogLevel::INFO;

    void setLevel(LogLevel level) {
        gLogLevel = level;
    }

    void log(std::string_view level, const Message& message, const std::source_location& sourceLocation) {
        std::cout << std::format(
                "[{}]: {}:{} {}",
                level,
                std::filesystem::path(sourceLocation.file_name()).filename().string(),
                sourceLocation.line(),
                message.data
            )
        << '\n';
    }

    void info(const Message& message, const std::source_location& loc) {
        if (gLogLevel >= LogLevel::INFO) log("INFO", message, loc);
    }

    void debug(const Message& message, const std::source_location& loc) {
        if (gLogLevel >= LogLevel::DEBUG) log("DEBUG", message, loc);
    }

    void trace(const Message& message, const std::source_location& loc) {
        if (gLogLevel >= LogLevel::TRACE) log("TRACE", message, loc);
    }

    void warning(const Message& message, const std::source_location& loc) {
        if (gLogLevel >= LogLevel::NO_WARNINGS) log("WARNING", message, loc);
    }

    void error(const char* message, const std::source_location& loc) {
        throw "NYI";
    }

    void error(const Message& message, const std::source_location& loc) {
        log("ERROR", message, loc);
        std::abort();
    }
} // ggm