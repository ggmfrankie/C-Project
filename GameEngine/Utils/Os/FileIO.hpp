//
// Created by ertls on 17.02.2026.
//

#pragma once

#include <filesystem>
#include <string>


namespace ggm::FileIO {
    using Path = std::filesystem::path;

    std::string readFile(const std::string& filePath);
    std::string readFile(const Path& filePath);
}
