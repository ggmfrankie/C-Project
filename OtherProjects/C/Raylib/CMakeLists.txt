cmake_minimum_required(VERSION 4.0)
project(Raylib LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
        "${CMAKE_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/Dependencies/raylib/src"
)

add_subdirectory(Dependencies/raylib)

add_executable(Raylib
        main.c
)

target_link_libraries(Raylib PRIVATE
        raylib_static
)

