//
// Created by Stefan on 22.03.2026.
//

#include "Logger.hpp"

namespace Engine {
    Logger& Logger::Get() {
        static Logger logger;

        return logger;
    }

    void Logger::info(std::string &s) {
    }

    void Logger::error(std::string &s) {
    }
} // Engine