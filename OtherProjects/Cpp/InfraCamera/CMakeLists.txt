cmake_minimum_required(VERSION 4.3)
project(InfraCamera)

set(CMAKE_CXX_STANDARD 23)
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/Dependencies/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)
add_subdirectory("${CMAKE_SOURCE_DIR}/Dependencies/EIPScanner")

add_executable(InfraCamera main.cpp
        Camera/Camera.cpp
        Camera/Camera.hpp)

target_include_directories(InfraCamera PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Dependencies/EIPScanner/src)
target_link_libraries(InfraCamera
        PRIVATE ${OpenCV_LIBS} EIPScannerS

)

# Make app runnable from IDE by copying OpenCV runtime DLLs beside the exe.
get_filename_component(OPENCV_LIB_DIR "${OpenCV_DIR}" ABSOLUTE)
get_filename_component(OPENCV_ARCH_DIR "${OPENCV_LIB_DIR}" DIRECTORY)
set(OPENCV_BIN_DIR "${OPENCV_ARCH_DIR}/bin")

if(EXISTS "${OPENCV_BIN_DIR}")
    add_custom_command(TARGET InfraCamera POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${OPENCV_BIN_DIR}"
                "$<TARGET_FILE_DIR:InfraCamera>"
        COMMENT "Copying OpenCV runtime DLLs")
endif()