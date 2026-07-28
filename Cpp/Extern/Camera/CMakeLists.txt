cmake_minimum_required(VERSION 4.3)
project(Camera)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        "${CMAKE_SOURCE_DIR}/Dependencies/cpp-httpLib/"
)

add_executable(Camera
        main.cpp
        Utils/Json.cpp
        Utils/Json.hpp
        Test/TicTacToe.cpp
        Test/TicTacToe.hpp
        Network/Server.cpp
        Network/Server.hpp
        Network/Client.cpp
        Network/Client.hpp
        Dependencies/cpp-httpLib/httplib.hpp
        Dependencies/cpp-httpLib/json.hpp
        Utils/McpInterface.cpp
        Utils/McpInterface.hpp
        Utils/LazyStream.hpp
        Utils/Utils.cpp
        Utils/Parsing.cpp
        Utils/Parsing.hpp)
