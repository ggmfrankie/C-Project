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
add_subdirectory(GUIEngine)

include_directories(
        "${CMAKE_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/Dependencies/include"
        "${CMAKE_SOURCE_DIR}/GameEngine"
        "${CMAKE_SOURCE_DIR}/GameEngine/Utils"
        "${CMAKE_SOURCE_DIR}/Dependencies/JoltPhysics"
)

add_executable(C-Project
        Dependencies/include/stb/stb_image.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_truetype.h
        Dependencies/include/stb/stb_rect_pack.h
        GameEngine/GameEngine.cpp
        Main.cpp
        GameEngine/Render/Screen.cpp
        GameEngine/Render/Screen.hpp
        GameEngine/Utils/Math/Vector.hpp
        GameEngine/Render/Shader/Shader.cpp
        GameEngine/Render/Shader/Shader.hpp
        GameEngine/Utils/Os/FileIO.cpp
        GameEngine/Utils/Os/FileIO.hpp
        GameEngine/Utils/Math/Matrix.hpp
        GameEngine/Render/Transformation/Camera.cpp
        GameEngine/Render/Transformation/Camera.hpp
        GameEngine/Utils/StringUtils.cpp
        GameEngine/Utils/StringUtils.hpp
        GameEngine/Utils/Math/Quaternion.hpp
        GameEngine/Utils/DataStorage/InlineVector.hpp
        GameEngine/Utils/ggmdef.hpp
        GameEngine/Games/IGame.hpp
        GameEngine/Engine/IO/Input.cpp
        GameEngine/Engine/IO/Input.hpp
        GameEngine/Games/BaseGame/BaseGame.cpp
        GameEngine/Games/BaseGame/BaseGame.hpp
        GameEngine/Games/BaseGame/GameGui.h
        GUIEngine/GuiInterface.h
        GameEngine/Engine/Task.cpp
        GameEngine/Engine/Task.hpp
        GameEngine/Games/BaseGame/GuiTasks.hpp
        GameEngine/Games/BaseGame/GuiTasks.cpp
        GameEngine/Engine/CommandRegistry.cpp
        GameEngine/Engine/CommandRegistry.hpp
        GameEngine/Render/Shader/Lighting/PointLight.cpp
        GameEngine/Render/Shader/Lighting/PointLight.hpp
        GameEngine/Render/Shader/DeferredShader.cpp
        GameEngine/Render/Shader/DeferredShader.hpp
        GameEngine/Games/Controller/Player.cpp
        GameEngine/Games/Controller/Player.hpp
        GameEngine/Utils/DataStorage/SparseSet.hpp
        GameEngine/Engine/Debug/Logger.cpp
        GameEngine/Engine/Debug/Logger.hpp
        GameEngine/Utils/Os/Time.cpp
        GameEngine/Utils/Os/Time.hpp
        GameEngine/Engine/Core/PhysixBox2D/PointMass.hpp
        GameEngine/Engine/Core/PhysixBox2D/PhysicsSystem.cpp
        GameEngine/Engine/Core/PhysixBox2D/PhysicsSystem.hpp
        GameEngine/Engine/Core/PhysixBox2D/SoftBody2D.cpp
        GameEngine/Engine/Core/PhysixBox2D/SoftBody2D.hpp
        GameEngine/Engine/Core/PhysixBox2D/Constrains/Spring.cpp
        GameEngine/Engine/Core/PhysixBox2D/Constrains/Spring.hpp
        GameEngine/Test/Test.cpp
        GameEngine/Test/Test.hpp
        GameEngine/Games/Scene/3D/Scene3D.cpp
        GameEngine/Games/Scene/3D/Scene3D.hpp
        GameEngine/Games/Scene/IScene.hpp
        GameEngine/Games/Scene/2D/Scene2D.cpp
        GameEngine/Games/Scene/2D/Scene2D.hpp
        GameEngine/Games/Game2D/Game2D.cpp
        GameEngine/Games/Game2D/Game2D.hpp
        GameEngine/Engine/Core/PhysixBox2D/Collision.hpp
        GameEngine/Test/Java.cpp
        GameEngine/Test/Java.hpp
        GameEngine/Utils/DataStorage/Stream.hpp
        GameEngine/Games/BaseGame/GameGui.c
        GameEngine/Utils/DataStorage/LazyStream.hpp
        GameEngine/Games/Scene/2D/LayerStack.cpp
        GameEngine/Games/Scene/2D/LayerStack.hpp
        GameEngine/Games/Scene/2D/ILayer.hpp
        GameEngine/Games/Game2D/DebugLayer.cpp
        GameEngine/Games/Game2D/DebugLayer.hpp
        EngineDefines.hpp
        GameEngine/Games/Game2D/Game2DGui.c
        GameEngine/Games/Game2D/Game2DGui.h
        GameEngine/Render/Objects/2D/Physics/PhysicsFactory.cpp
        GameEngine/Render/Objects/2D/Physics/PhysicsObjectRef.cpp
        GameEngine/Render/Objects/2D/Render/DynamicMesh.cpp
        GameEngine/Render/Objects/2D/Render/Renderer.cpp
        GameEngine/Render/Objects/2D/Render/RenderObject.cpp
        GameEngine/Render/Objects/2D/GameObject.cpp
        GameEngine/Render/Objects/3D/Loader/OBJLoader.cpp
        GameEngine/Render/Objects/3D/Physics/PhysicsFactory.cpp
        GameEngine/Render/Objects/3D/Physics/PhysicsHandler3D.cpp
        GameEngine/Render/Objects/3D/Physics/PhysicsObjectRef.cpp
        GameEngine/Render/Objects/3D/Render/Material.cpp
        GameEngine/Render/Objects/3D/Render/Mesh.cpp
        GameEngine/Render/Objects/3D/Render/Renderer3D.cpp
        GameEngine/Render/Objects/3D/Render/RenderObject3D.cpp
        GameEngine/Render/Objects/3D/Render/Texture.cpp
        GameEngine/Render/Objects/3D/GameObject.cpp
        GameEngine/Utils/Async/Coroutine.cpp
        GameEngine/Utils/Async/Coroutine.hpp
        GameEngine/Utils/Math/Math.hpp
        OtherProjects/C/Extern/CVector.h
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
