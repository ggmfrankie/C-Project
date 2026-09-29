cmake_minimum_required(VERSION 4.0)

project(ModulesTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ModulesTest
        mein.cpp
)

target_sources(ModulesTest
        PRIVATE
        FILE_SET CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        String.cppm
)