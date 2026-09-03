cmake_minimum_required(VERSION 4.0)
project(C-Project LANGUAGES C CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g -pthread")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Dependencies/JoltPhysics/Build)
target_compile_definitions(Jolt PUBLIC JPH_ENABLE_ASSERTS)
target_compile_options(Jolt PRIVATE -fno-lto -Wno-array-bounds)

add_subdirectory(OtherProjects/Cpp/McpServer)

include_directories(
        "${CMAKE_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/Dependencies/include"
        "${CMAKE_SOURCE_DIR}/C"
        "${CMAKE_SOURCE_DIR}/C/Utils"
        "${CMAKE_SOURCE_DIR}/Cpp"
        "${CMAKE_SOURCE_DIR}/Cpp/Utils"
        "${CMAKE_SOURCE_DIR}/Dependencies/JoltPhysics"
)

add_library(GUI STATIC
        Main.c
        C/Utils/_Deprecated_/CString_v1.c
        C/Utils/_Deprecated_/CString_v1.h
        Dependencies/GL/gl.c
        C/Utils/Math/Vector.c
        C/Utils/Math/Vector.h
        C/Utils/_Deprecated_/HashMap.h
        C/Utils/Os/FileIO.c
        C/Utils/Os/FileIO.h
        C/Utils/_Deprecated_/StringBuilder.c
        C/Utils/_Deprecated_/StringBuilder.h
        C/Utils/Makros/Makros.h
        C/Utils/Os/Time.c
        C/Utils/Os/Time.h
        C/Utils/_NYI_/MemoryArena.c
        C/_Projects_/Chess/ChessGame.c
        C/_Projects_/Chess/ChessGame.h
        C/Utils/Os/Network.c
        C/Utils/Os/Network.h
        C/Utils/Misc/UtilityFun.c
        C/Utils/Misc/UtilityFun.h
        C/_Projects_/Test/TestGUI.c
        C/_Projects_/ChessNewandImproved/ChessBetter2.c
        C/_Projects_/ChessNewandImproved/ChessBetter2.c
        C/_Projects_/Tokenizer/Calculator.c
        C/_Projects_/Tokenizer/Calculator.h
        C/Utils/DataStructures/CArrayList.h
        C/_Projects_/TFE/TFE.c
        C/_Projects_/TFE/TFE.h
        C/GuiTypes.h
        C/GuiDefines.h
        C/Utils/DataStructures/CHashMap.h
        C/Utils/_Deprecated_/Test.h
        C/Utils/DataStructures/CHashMap.c
        C/Utils/Typedef.h
        C/Utils/Makros/Defer.c
        C/Utils/DataStructures/CString.c
        C/Utils/DataStructures/CString.h
        C/Utils/Logging/Logging.h
        OtherProjects/C/Informatik/DrunkenBishop.c
        OtherProjects/C/Informatik/GameBoard.c
        C/Utils/Makros/NamedParameters.h
        C/Utils/Test.c
        C/Utils/DataStructures/CSparseSet.c
        C/Utils/DataStructures/CSparseSet.h
        C/Utils/Makros/Defer.h
        C/Utils/Test.h
        C/Utils/Makros/Helper.h
        C/Utils/DataStructures/CArrayList.c
        C/GUI/Drawing/Mesh/Mesh.c
        C/GUI/Drawing/Mesh/Mesh.h
        C/GUI/Drawing/Shader/ComputeShader.c
        C/GUI/Drawing/Shader/ComputeShader.h
        C/GUI/Drawing/Shader/Shader.c
        C/GUI/Drawing/Shader/Shader.h
        C/GUI/Drawing/Texture/Texture.c
        C/GUI/Drawing/Texture/Texture.h
        C/GUI/Drawing/Render.c
        C/GUI/Drawing/Render.h
        C/GUI/Drawing/RenderTypes.h
        C/GUI/Drawing/Text.c
        C/GUI/Drawing/Text.h
        C/GUI/GuiElement/ElementTypes/Scrollbar.c
        C/GUI/GuiElement/ElementTypes/Scrollbar.h
        C/GUI/GuiElement/ElementTypes/TextField.c
        C/GUI/GuiElement/ElementTypes/TextField.h
        C/GUI/GuiElement/IO/Tasks.c
        C/GUI/GuiElement/IO/Tasks.h
        C/GUI/GuiElement/IO/Update.c
        C/GUI/GuiElement/IO/Update.h
        C/GUI/GuiElement/CallbackHelper.c
        C/GUI/GuiElement/CallbackHelper.h
        C/GUI/GuiElement/GuiElement.c
        C/GUI/GuiElement/GuiElement.h
        C/GUI/Layout/Layout.c
        C/GUI/Layout/Layout.h
        C/GUI/Engine.c
        C/GUI/Engine.h
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
        Cpp/Utils/Os/FileIO.cpp
        Cpp/Utils/Os/FileIO.hpp
        Cpp/Utils/Math/Matrix.hpp
        Cpp/Render/Transformation/Camera.cpp
        Cpp/Render/Transformation/Camera.hpp
        Cpp/Utils/StringUtils.cpp
        Cpp/Utils/StringUtils.hpp
        Cpp/Utils/Math/Quaternion.hpp
        Cpp/Utils/DataStorage/InlineVector.hpp
        Cpp/Utils/ggmdef.hpp
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
        Cpp/Utils/Os/Time.cpp
        Cpp/Utils/Os/Time.hpp
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
        Cpp/Utils/Math/Math.hpp
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
        McpServerLib
)
endif (WIN32)

if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(C-Project PRIVATE
            glfw
            GUI
            Jolt
            OpenGL::OpenGL
            McpServerLib
            #TBB::tbb
            #RustGame
    )
endif (UNIX)
