cmake_minimum_required(VERSION 4.0)
project(C C)

set(CMAKE_C_STANDARD 11)

include_directories("${CMAKE_SOURCE_DIR}/Dependencies/include")

add_executable(C
        Utils/GenericLinkedList.h
        Main.c
        Utils/String.c
        Utils/String.h
        Render/Render.c
        Render/Render.h
        Render/Mesh.c
        Render/Mesh.h
        Utils/ArrayList.h
        Render/gl.c
        Render/Engine.c
        Render/Engine.h
        Utils/Vector.c
        Utils/Vector.h
        Utils/DataStructures.c
        Render/Shader.c
        Render/Shader.h
        Utils/HashMap.h
        Utils/Map.h
        Utils/FileIO.c
        Utils/FileIO.h
        Utils/StringBuilder.c
        Utils/StringBuilder.h
        Render/GuiElement.c
        Render/GuiElement.h
        Utils/SimpleArray.h
        Render/ComputeShader.c
        Render/ComputeShader.h
        Render/Texture.c
        Render/Texture.h
)
#add_subdirectory("${CMAKE_SOURCE_DIR}/Render")

set(GLFW_LIB "${CMAKE_SOURCE_DIR}/Dependencies/lib-mingw-w64/libglfw3.a")

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB}"
)

target_link_libraries(C
        glfw
        opengl32     # OpenGL (needed by GLFW)
        gdi32        # Windows GDI
        user32       # Window and input handling
        kernel32     # Core Windows functions
)