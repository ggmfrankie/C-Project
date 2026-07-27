//
// Created by ertls on 27.07.2026.
//
#pragma once
#include <string>
#include <vector>

namespace Utils {
    class Json {
        explicit Json(std::string data);
        Json(std::string name, int value);
        Json(std::string name, std::string value);
        Json(std::string name, bool value);

    private:
        
    };
} // Utils

