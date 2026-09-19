cmake_minimum_required(VERSION 4.0)
project(InfraCamera)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Dependencies/EIPScanner")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../../../GUIEngine" "${CMAKE_BINARY_DIR}/C")

add_executable(InfraCamera main.cpp
        Camera/Camera.hpp
        CameraGUI/CameraGUI.c
        CameraGUI/CameraGUI.h
        CameraGUI/CameraController.cpp
        CameraGUI/CameraController.hpp)

target_include_directories(InfraCamera PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/Dependencies/EIPScanner/src")

target_link_libraries(InfraCamera
        EIPScannerS
        GUI
)