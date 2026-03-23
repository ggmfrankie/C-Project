//
// Created by Stefan on 22.03.2026.
//

#pragma once
#include <string>
#include <vector>
#include <bits/chrono.h>

namespace Engine {
    class Logger {
        Logger() = default;

        struct Entry {
            std::string info;

        };

        std::vector<std::string> logs;
    public:
        static Logger &Get();

        void info(std::string& s);
        void error(std::string& s);
    };
} // Engine