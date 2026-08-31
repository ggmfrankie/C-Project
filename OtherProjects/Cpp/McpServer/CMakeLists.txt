cmake_minimum_required(VERSION 4.2)
project(McpServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/cpp-httpLib"
        "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/json"
)

add_library(McpServerLib STATIC
    Network/Server.cpp
    Network/Client.cpp
    Utils/Utils.cpp
    Utils/Parsing.cpp
    Mcp/ReturnTypes.cpp
)

add_executable(McpServer
    main.cpp
    Test/TicTacToe.cpp
)

if (WIN32)
        target_link_libraries(McpServerLib PUBLIC ws2_32)
        target_link_libraries(McpServer PRIVATE McpServerLib)
endif()
