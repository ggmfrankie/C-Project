//
// Created by Stefan on 10.11.2026.
//

#pragma once
#include <exception>
#include <format>

namespace ggm {
    class Exception : public std::exception {
        std::string mMessage;
    public:
        template<typename... Args>
        explicit Exception(std::format_string<Args...> fmt, Args&&... args) :
            mMessage(std::format(fmt, std::forward<Args>(args)...))
        {}

        // explicit Exception(const std::string& msg) :
        //     mMessage(msg)
        // {}
    };
} // ggm
