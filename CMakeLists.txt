cmake_minimum_required(VERSION 4.0)
project(MixedProject LANGUAGES C CXX)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -march=native -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -pthread")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include_directories("${CMAKE_SOURCE_DIR}/Dependencies/include")

add_executable(C-Project
        C/Utils/GenericLinkedList.h
        Main.c
        C/Utils/CString.c
        C/Utils/CString.h
        C/Render/Render.c
        C/Render/Render.h
        C/Render/GUI/Mesh.c
        C/Render/GUI/Mesh.h
        C/Utils/ArrayList.h
        C/Render/gl.c
        C/Render/Engine.c
        C/Render/Engine.h
        C/Utils/Vector.c
        C/Utils/Vector.h
        C/Utils/DataStructures.h
        C/Render/Shader.c
        C/Render/Shader.h
        C/Utils/HashMap.h
        C/Utils/Map.h
        C/Utils/FileIO.c
        C/Utils/FileIO.h
        C/Utils/StringBuilder.c
        C/Utils/StringBuilder.h
        C/Render/GUI/GuiElement.c
        C/Render/GUI/GuiElement.h
        C/Utils/SimpleArray.h
        C/Render/ComputeShader.c
        C/Render/ComputeShader.h
        C/Render/GUI/Texture.c
        C/Render/GUI/Texture.h
        C/Extern/Informatik/Spannungsteiler_A3.c
        C/Extern/Informatik/Spannungsteiler_A3.h
        C/Extern/Informatik/DrunkenBishop.c
        C/Extern/Informatik/DrunkenBishop.h
        C/Utils/Matrix.h
        C/Extern/Informatik/GameBoard.c
        C/Extern/Informatik/GameBoard.h
        C/Utils/Makros.h
        C/Utils/TimeMeasurenments.c
        C/Utils/TimeMeasurenments.h
        C/Render/GUI/CallbackFunctions.h
        C/Render/GUI/CallbackFunctions.c
        Dependencies/include/stb/stb_image.h
        C/Extern/Informatik/Geldautomat.c
        C/Render/GUI/TextDisplaying.c
        C/Render/GUI/TextDisplaying.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_rect_pack.h
        C/Extern/Informatik/PointerFun.c
        C/Extern/Informatik/Namensliste.c
        C/Extern/Informatik/Personalverwaltung.c
        C/Render/GUI/Tasks.c
        C/Render/GUI/Tasks.h
        C/Extern/Informatik/MonteCarlo.c
        C/Extern/Informatik/MonteCarlo.h
        C/Extern/Informatik/libminibmp.c
        C/Utils/MemoryArena.c
        C/Render/GUI/GuiElementData.c
        C/Render/GUI/GuiElementData.h
        C/Extern/Libraries/WinAdapt.c
        C/Extern/Libraries/WinAdapt.h
        C/Extern/Informatik/TowersOfHanoi.c
        C/Extern/Informatik/TowersOfHanoi.h
        C/Projects/Chess/ChessGame.c
        C/Projects/Chess/ChessGame.h
        C/Utils/Network.c
        C/Utils/Network.h
        C/Utils/UtilityFun.c
        C/Utils/UtilityFun.h
        C/Projects/Test/TestGUI.c
        C/Projects/ChessNewandImproved/ChessBetter2.c
        C/Projects/ChessNewandImproved/ChessBetter2.c
        C/Projects/Tokenizer/Calculator.c
        C/Projects/Tokenizer/Calculator.h
        Cpp/GameEngine.cpp
        Main.cpp
        Cpp/Render/Screen.cpp
        Cpp/Render/Screen.h
        Cpp/Render/Objects/Geometry/Mesh.cpp
        Cpp/Render/Objects/Geometry/Mesh.h
        Cpp/Utils/Math/Vector.h
        Cpp/Render/Shader/Shader.cpp
        Cpp/Render/Shader/Shader.h
        C/Render/CRenderInterface.h
        Cpp/Utils/FileIO.cpp
        Cpp/Utils/FileIO.h
        Cpp/Render/Objects/Object.cpp
        Cpp/Render/Objects/Object.h
        Cpp/Utils/Math/Matrix.h
        Cpp/Render/Transformation/Camera.cpp
        Cpp/Render/Transformation/Camera.h
        Cpp/Render/Objects/Geometry/Texture.cpp
        Cpp/Render/Objects/Geometry/Texture.h
        Cpp/Render/Objects/Geometry/Material.cpp
        Cpp/Render/Objects/Geometry/Material.h
        Cpp/Render/Objects/Loader/ObjLoader.cpp
        Cpp/Render/Objects/Loader/ObjLoader.h
        Cpp/Utils/Utils.cpp
        Cpp/Utils/Utils.h
        Cpp/Utils/Math/Quaternion.h
        Cpp/Utils/DataStorage/ArrayList.h
        Cpp/Utils/Math/ggmdef.h
        Cpp/Games/IGame.h
        Cpp/Render/IO/Input.cpp
        Cpp/Render/IO/Input.h
        Cpp/Games/BaseGame.cpp
        Cpp/Games/BaseGame.h
        C/Utils/CArrayList.c
        C/Utils/CArrayList.h
)
#add_subdirectory("${CMAKE_SOURCE_DIR}/Render")

set(GLFW_LIB "${CMAKE_SOURCE_DIR}/Dependencies/lib-mingw-w64/libglfw3.a")

add_library(glfw STATIC IMPORTED)

set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB}"
)



target_link_libraries(C-Project
        glfw
        opengl32     # OpenGL (needed by GLFW)
        gdi32        # Windows GDI
        user32       # Window and input handling
        kernel32     # Core Windows functions
        ws2_32
)
