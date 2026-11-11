//
// Created by Stefan on 10.11.2026.
//

#pragma once
#include <exception>
#include <format>
#include <source_location>
#include <filesystem>

#include "Message.hpp"

namespace ggm {
    class Exception : public std::exception {
        std::string mMessage;
    public:

        explicit Exception(Message&& message, const std::source_location& loc = std::source_location::current()) :
        mMessage(
            std::format(
                "[EXCEPTION]: {}:{} {}",
                std::filesystem::path(loc.file_name()).filename().string(),
                loc.line(),
                message.data
            )
        )
        {}
    };
} // ggm
