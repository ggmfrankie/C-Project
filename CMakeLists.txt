cmake_minimum_required(VERSION 4.0)
project(C-Project LANGUAGES C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -pthread")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Dependencies/JoltPhysics/Build)
target_compile_definitions(Jolt PUBLIC JPH_ENABLE_ASSERTS)
target_compile_options(Jolt PRIVATE -fno-lto -Wno-array-bounds)

include_directories(
        "${CMAKE_SOURCE_DIR}/Dependencies/include"
        "${CMAKE_SOURCE_DIR}/C"
        "${CMAKE_SOURCE_DIR}/Cpp"
        "${CMAKE_SOURCE_DIR}/Dependencies/JoltPhysics"
)

add_library(GUI STATIC
        C/Utils/_Deprecated_/GenericLinkedList.h
        Main.c
        C/Utils/_Deprecated_/CString_v1.c
        C/Utils/_Deprecated_/CString_v1.h
        C/Render/Drawing/Render.c
        C/Render/Drawing/Render.h
        C/Render/Drawing/Mesh.c
        C/Render/Drawing/Mesh.h
        Dependencies/GL/gl.c
        C/Render/Engine.c
        C/Render/Engine.h
        C/Utils/Math/Vector.c
        C/Utils/Math/Vector.h
        C/Render/Shader.c
        C/Render/Shader.h
        C/Utils/_Deprecated_/HashMap.h
        C/Utils/Os/FileIO.c
        C/Utils/Os/FileIO.h
        C/Utils/_Deprecated_/StringBuilder.c
        C/Utils/_Deprecated_/StringBuilder.h
        C/Render/GUI/GuiElement.c
        C/Render/GUI/GuiElement.h
        C/Render/ComputeShader.c
        C/Render/ComputeShader.h
        C/Render/GUI/Texture.c
        C/Render/GUI/Texture.h
        C/Utils/Makros/Makros.h
        C/Utils/Os/Time.c
        C/Utils/Os/Time.h
        C/Render/GUI/CallbackFunctions.h
        C/Render/GUI/CallbackFunctions.c
        C/Render/Drawing/TextDisplaying.c
        C/Render/Drawing/TextDisplaying.h
        C/Render/GUI/Tasks.c
        C/Render/GUI/Tasks.h
        C/Utils/_NYI_/MemoryArena.c
        C/Render/GUI/GuiElementData.c
        C/Render/GUI/GuiElementData.h
        C/Projects/Chess/ChessGame.c
        C/Projects/Chess/ChessGame.h
        C/Utils/Os/Network.c
        C/Utils/Os/Network.h
        C/Utils/Misc/UtilityFun.c
        C/Utils/Misc/UtilityFun.h
        C/Projects/Test/TestGUI.c
        C/Projects/ChessNewandImproved/ChessBetter2.c
        C/Projects/ChessNewandImproved/ChessBetter2.c
        C/Projects/Tokenizer/Calculator.c
        C/Projects/Tokenizer/Calculator.h
        C/Utils/DataStructures/CArrayList.h
        C/Projects/TFE/TFE.c
        C/Projects/TFE/TFE.h
        C/GuiTypes.h
        C/Render/GUI/Update.c
        C/Render/GUI/Update.h
        C/Render/Drawing/Batcher.c
        C/Render/Drawing/Batcher.h
        C/Render/Drawing/RenderTypes.h
        C/GuiDefines.h
        C/Utils/DataStructures/CHashMap.h
        C/Utils/DataStructures/Test.h
        C/Utils/DataStructures/CHashMap.c
        C/Utils/Typedef.h
        C/Utils/Makros/Defer.c
        C/Utils/DataStructures/CString.c
        C/Utils/DataStructures/CString.h
        C/Utils/Logging/Logging.c
        C/Utils/Logging/Logging.h
        OtherProjects/C/Informatik/DrunkenBishop.c
        OtherProjects/C/Informatik/GameBoard.c
        C/Utils/Makros/NamedParameters.h
        C/Render/GUI/ElementTypes/TextField.c
        C/Utils/Test.c
        C/Utils/DataStructures/CSparseSet.c
        C/Utils/DataStructures/CSparseSet.h
        C/Utils/Makros/Defer.h
        C/Utils/Test.h
)

set_target_properties(GUI PROPERTIES LINKER_LANGUAGE C)

add_executable(C-Project
        Dependencies/include/stb/stb_image.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_rect_pack.h
        Cpp/GameEngine.cpp
        Main.cpp
        Cpp/Render/Screen.cpp
        Cpp/Render/Screen.hpp
        Cpp/Utils/Math/Vector.hpp
        Cpp/Render/Shader/Shader.cpp
        Cpp/Render/Shader/Shader.hpp
        Cpp/Utils/FileIO.cpp
        Cpp/Utils/FileIO.hpp
        Cpp/Utils/Math/Matrix.hpp
        Cpp/Render/Transformation/Camera.cpp
        Cpp/Render/Transformation/Camera.hpp
        Cpp/Utils/Utils.cpp
        Cpp/Utils/Utils.hpp
        Cpp/Utils/Math/Quaternion.hpp
        Cpp/Utils/DataStorage/InlineVector.hpp
        Cpp/Utils/Math/ggmdef.hpp
        Cpp/Games/IGame.hpp
        Cpp/Engine/IO/Input.cpp
        Cpp/Engine/IO/Input.hpp
        Cpp/Games/BaseGame/BaseGame.cpp
        Cpp/Games/BaseGame/BaseGame.hpp
        Cpp/Games/BaseGame/GameGui.h
        C/GuiInterface.h
        C/GuiInterface.h
        Cpp/Engine/Task.cpp
        Cpp/Engine/Task.hpp
        Cpp/Games/BaseGame/GuiTasks.hpp
        Cpp/Games/BaseGame/GuiTasks.cpp
        Cpp/Engine/CommandRegistry.cpp
        Cpp/Engine/CommandRegistry.hpp
        Cpp/Render/Shader/Lighting/PointLight.cpp
        Cpp/Render/Shader/Lighting/PointLight.hpp
        Cpp/Render/Shader/DeferredShader.cpp
        Cpp/Render/Shader/DeferredShader.hpp
        Cpp/Games/Controller/Player.cpp
        Cpp/Games/Controller/Player.hpp
        Cpp/Utils/DataStorage/SparseSet.hpp
        Cpp/Engine/Debug/Logger.cpp
        Cpp/Engine/Debug/Logger.hpp
        Cpp/Utils/Time.cpp
        Cpp/Utils/Time.hpp
        Cpp/Engine/Core/PhysixBox2D/PointMass.hpp
        Cpp/Engine/Core/PhysixBox2D/PhysicsSystem.cpp
        Cpp/Engine/Core/PhysixBox2D/PhysicsSystem.hpp
        Cpp/Engine/Core/PhysixBox2D/SoftBody2D.cpp
        Cpp/Engine/Core/PhysixBox2D/SoftBody2D.hpp
        Cpp/Engine/Core/PhysixBox2D/Constrains/Spring.cpp
        Cpp/Engine/Core/PhysixBox2D/Constrains/Spring.hpp
        Cpp/Test/Test.cpp
        Cpp/Test/Test.hpp
        Cpp/Games/Scene/3D/Scene3D.cpp
        Cpp/Games/Scene/3D/Scene3D.hpp
        Cpp/Games/Scene/IScene.hpp
        Cpp/Games/Scene/2D/Scene2D.cpp
        Cpp/Games/Scene/2D/Scene2D.hpp
        Cpp/Games/Game2D/Game2D.cpp
        Cpp/Games/Game2D/Game2D.hpp
        Cpp/Engine/Core/PhysixBox2D/Collision.hpp
        Cpp/Test/Java.cpp
        Cpp/Test/Java.hpp
        Cpp/Utils/DataStorage/Stream.hpp
        Cpp/Games/BaseGame/GameGui.c
        Cpp/Utils/DataStorage/LazyStream.hpp
        Cpp/Games/Scene/2D/LayerStack.cpp
        Cpp/Games/Scene/2D/LayerStack.hpp
        Cpp/Games/Scene/2D/ILayer.hpp
        Cpp/Games/Game2D/DebugLayer.cpp
        Cpp/Games/Game2D/DebugLayer.hpp
        EngineDefines.hpp
        Cpp/Games/Game2D/Game2DGui.c
        Cpp/Games/Game2D/Game2DGui.h
        Cpp/Render/Objects/2D/Physics/PhysicsFactory.cpp
        Cpp/Render/Objects/2D/Physics/PhysicsObjectRef.cpp
        Cpp/Render/Objects/2D/Render/DynamicMesh.cpp
        Cpp/Render/Objects/2D/Render/Renderer.cpp
        Cpp/Render/Objects/2D/Render/RenderObject.cpp
        Cpp/Render/Objects/2D/GameObject.cpp
        Cpp/Render/Objects/3D/Loader/OBJLoader.cpp
        Cpp/Render/Objects/3D/Physics/PhysicsFactory.cpp
        Cpp/Render/Objects/3D/Physics/PhysicsHandler3D.cpp
        Cpp/Render/Objects/3D/Physics/PhysicsObjectRef.cpp
        Cpp/Render/Objects/3D/Render/Material.cpp
        Cpp/Render/Objects/3D/Render/Mesh.cpp
        Cpp/Render/Objects/3D/Render/Renderer3D.cpp
        Cpp/Render/Objects/3D/Render/RenderObject3D.cpp
        Cpp/Render/Objects/3D/Render/Texture.cpp
        Cpp/Render/Objects/3D/GameObject.cpp
        Cpp/Utils/Async/Coroutine.cpp
        Cpp/Utils/Async/Coroutine.hpp
)

target_compile_definitions(C-Project PUBLIC JPH_ENABLE_ASSERTS)

if (WIN32)
set(GLFW_LIB "${CMAKE_SOURCE_DIR}/Dependencies/lib-mingw-w64/libglfw3.a")
endif (WIN32)

if (UNIX)
    set(GLFW_LIB "${CMAKE_SOURCE_DIR}/Dependencies/lib-mingw-w64/Linux/libglfw3.a")
endif (UNIX)
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB}"
)


#find_package(TBB REQUIRED)

add_library(RustGame SHARED IMPORTED)

set_target_properties(RustGame PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/RustGame/game.dll"
        IMPORTED_IMPLIB   "${CMAKE_SOURCE_DIR}/Dependencies/RustGame/game.dll.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/RustGame"
)

add_custom_command(TARGET C-Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Dependencies/RustGame/game.dll"
        $<TARGET_FILE_DIR:C-Project>
)

if (WIN32)
target_link_libraries(C-Project PRIVATE
        glfw
        opengl32     # OpenGL (needed by GLFW)
        gdi32        # Windows GDI
        user32       # Window and input handling
        kernel32     # Core Windows functions
        ws2_32
        GUI
        Jolt
        winpthread
        #TBB::tbb
)
endif (WIN32)

if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(C-Project PRIVATE
            glfw
            GUI
            Jolt
            OpenGL::OpenGL
            #TBB::tbb
            #RustGame
    )
endif (UNIX)
