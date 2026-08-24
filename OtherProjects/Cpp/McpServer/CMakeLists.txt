cmake_minimum_required(VERSION 4.3)
project(McpServer)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        "${CMAKE_SOURCE_DIR}/Dependencies/cpp-httpLib/"
        "${CMAKE_SOURCE_DIR}/Dependencies/json/"
)

add_executable(McpServer
        main.cpp
        Test/TicTacToe.cpp
        Test/TicTacToe.hpp
        Network/Server.cpp
        Network/Server.hpp
        Network/Client.cpp
        Network/Client.hpp
        Dependencies/cpp-httpLib/httplib.hpp
        Dependencies/json/json.hpp
        Mcp/McpFunctionRegistry.hpp
        Utils/LazyStream.hpp
        Utils/Utils.cpp
        Utils/Parsing.cpp
        Utils/Parsing.hpp
        Mcp/ReturnTypes.cpp
        Mcp/ReturnTypes.hpp)
