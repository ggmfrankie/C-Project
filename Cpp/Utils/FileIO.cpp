//
// Created by ertls on 17.02.2026.
//

#include "FileIO.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <sstream>

namespace ggm::FileIO {
    std::string readFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::ostringstream error;
            error << "Failed to open file at " << filePath << "\n";
            throw std::runtime_error(error.str());
        }

        return {
        std::istreambuf_iterator(file),
        std::istreambuf_iterator<char>()
        };

    }

    std::string readFile(const std::filesystem::path& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::ostringstream error;
            error << "Failed to open file at " << filePath << "\n";
            throw std::runtime_error(error.str());
        }

        return {
            std::istreambuf_iterator(file),
            std::istreambuf_iterator<char>()
        };

    }
}
