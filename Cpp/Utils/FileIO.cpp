//
// Created by ertls on 17.02.2026.
//

#include "FileIO.hpp"

#include <fstream>
#include <stdexcept>

namespace Utils::FileIO {
    std::string readFile(const std::string& filePath) {

        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            printf("Tried to open file %s\n", filePath.c_str());
            throw std::runtime_error("Failed to open file");
        }

        return std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );

    }
}
