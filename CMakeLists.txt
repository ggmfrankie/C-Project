cmake_minimum_required(VERSION 4.0)
project(C C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -march=native -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -lpthread")

set(CMAKE_C_STANDARD 11)

include_directories("${CMAKE_SOURCE_DIR}/Dependencies/include")

add_executable(C
        Utils/GenericLinkedList.h
        Main.c
        Utils/String.c
        Utils/String.h
        Render/Render.c
        Render/Render.h
        Render/GUI/Mesh.c
        Render/GUI/Mesh.h
        Utils/ArrayList.h
        Render/gl.c
        Render/Engine.c
        Render/Engine.h
        Utils/Vector.c
        Utils/Vector.h
        Utils/DataStructures.h
        Render/Shader.c
        Render/Shader.h
        Utils/HashMap.h
        Utils/Map.h
        Utils/FileIO.c
        Utils/FileIO.h
        Utils/StringBuilder.c
        Utils/StringBuilder.h
        Render/GUI/GuiElement.c
        Render/GUI/GuiElement.h
        Utils/SimpleArray.h
        Render/ComputeShader.c
        Render/ComputeShader.h
        Render/GUI/Texture.c
        Render/GUI/Texture.h
        Extern/Informatik/Spannungsteiler_A3.c
        Extern/Informatik/Spannungsteiler_A3.h
        Extern/Informatik/DrunkenBishop.c
        Extern/Informatik/DrunkenBishop.h
        Utils/Matrix.h
        Extern/Informatik/GameBoard.c
        Extern/Informatik/GameBoard.h
        Utils/Makros.h
        Utils/TimeMeasurenments.c
        Utils/TimeMeasurenments.h
        Render/GUI/CallbackFunctions.h
        Render/GUI/CallbackFunctions.c
        Dependencies/include/stb/stb_image.h
        Extern/Informatik/Geldautomat.c
        Render/GUI/TextDisplaying.c
        Render/GUI/TextDisplaying.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_rect_pack.h
        Extern/Informatik/PointerFun.c
        Extern/Informatik/Namensliste.c
        Extern/Informatik/Personalverwaltung.c
        Render/GUI/Tasks.c
        Render/GUI/Tasks.h
        Extern/Informatik/MonteCarlo.c
        Extern/Informatik/MonteCarlo.h
        Extern/Informatik/libminibmp.c
        Utils/MemoryArena.c
        Render/GUI/GuiElementData.c
        Render/GUI/GuiElementData.h
        Extern/Libraries/WinAdapt.c
        Extern/Libraries/WinAdapt.h
        Extern/Informatik/TowersOfHanoi.c
        Extern/Informatik/TowersOfHanoi.h
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