cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -pthread")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(GUI STATIC
        Main.c
        Utils/_Deprecated_/CString_v1.c
        Utils/_Deprecated_/CString_v1.h
        ../Dependencies/GL/gl.c
        Utils/Math/Vector.c
        Utils/Math/Vector.h
        Utils/_Deprecated_/HashMap.h
        Utils/Os/FileIO.c
        Utils/Os/FileIO.h
        Utils/_Deprecated_/StringBuilder.c
        Utils/_Deprecated_/StringBuilder.h
        Utils/Macros/Utils.h
        Utils/Os/Time.c
        Utils/Os/Time.h
        Utils/_NYI_/MemoryArena.c
        _Projects_/Chess/ChessGame.c
        _Projects_/Chess/ChessGame.h
        Utils/Os/Network.c
        Utils/Os/Network.h
        Utils/Misc/UtilityFun.c
        Utils/Misc/UtilityFun.h
        _Projects_/Test/TestGUI.c
        _Projects_/ChessNewandImproved/ChessBetter2.c
        _Projects_/ChessNewandImproved/ChessBetter2.c
        _Projects_/Tokenizer/Calculator.c
        _Projects_/Tokenizer/Calculator.h
        Utils/DataStructures/CArrayList.h
        _Projects_/TFE/TFE.c
        _Projects_/TFE/TFE.h
        GuiTypes.h
        GuiDefines.h
        Utils/DataStructures/CHashMap.h
        Utils/_Deprecated_/Test.h
        Utils/DataStructures/CHashMap.c
        Utils/Typedef.h
        Utils/Macros/Defer.c
        Utils/DataStructures/CString.c
        Utils/DataStructures/CString.h
        Utils/Logging/Logging.h
        Utils/_Deprecated_/NamedParameters.h
        Utils/Test.c
        Utils/DataStructures/CSparseSet.c
        Utils/DataStructures/CSparseSet.h
        Utils/Macros/Defer.h
        Utils/Test.h
        Utils/Macros/Macros.h
        Utils/DataStructures/CArrayList.c
        GUI/Drawing/Mesh/Mesh.c
        GUI/Drawing/Mesh/Mesh.h
        GUI/Drawing/Shader/ComputeShader.c
        GUI/Drawing/Shader/ComputeShader.h
        GUI/Drawing/Shader/Shader.c
        GUI/Drawing/Shader/Shader.h
        GUI/Drawing/Texture/Texture.c
        GUI/Drawing/Texture/Texture.h
        GUI/Drawing/Render.c
        GUI/Drawing/Render.h
        GUI/Drawing/RenderTypes.h
        GUI/Drawing/Text.c
        GUI/Drawing/Text.h
        GUI/GuiElement/ElementTypes/Scrollbar.c
        GUI/GuiElement/ElementTypes/Scrollbar.h
        GUI/GuiElement/ElementTypes/TextField.c
        GUI/GuiElement/ElementTypes/TextField.h
        GUI/GuiElement/IO/Tasks.c
        GUI/GuiElement/IO/Tasks.h
        GUI/GuiElement/IO/Update.c
        GUI/GuiElement/IO/Update.h
        GUI/GuiElement/CallbackHelper.c
        GUI/GuiElement/CallbackHelper.h
        GUI/GuiElement/GuiElement.c
        GUI/GuiElement/GuiElement.h
        GUI/Layout/Layout.c
        GUI/Layout/Layout.h
        GUI/Engine.c
        GUI/Engine.h
        GUI/GuiElement/ElementTypes/ScrollArea.c
        GUI/GuiElement/ElementTypes/ScrollArea.h
        GUI/GuiElement/ElementTypes.h
        GuiColors.h
        Utils/DataStructures/CStringView.c
        Utils/DataStructures/CStringView.h
        _Projects_/ExampleGUI/ExampleGUI.c
        _Projects_/ExampleGUI/ExampleGUI.h
        GUI/GuiElement/ElementTypes/TextArea.c
        GUI/GuiElement/ElementTypes/TextArea.h
        Utils/Logging/Logging.c
        Utils/Json/CJson.c
        Utils/Json/CJson.h
        Utils/_NYI_/String.c
        Utils/_NYI_/String.h
)

set_target_properties(GUI PROPERTIES LINKER_LANGUAGE C)

target_include_directories(GUI PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}/.."
        "${CMAKE_CURRENT_LIST_DIR}/../Dependencies/include"
)

target_compile_definitions(GUI PUBLIC
        GUI_PROJECT_SOURCE_DIR="${CMAKE_CURRENT_LIST_DIR}"
)

if (WIN32)
    add_library(gui_glfw STATIC IMPORTED)
    set_target_properties(gui_glfw PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../Dependencies/lib-mingw-w64/libglfw3.a"
    )
endif (WIN32)

if (UNIX)
    add_library(gui_glfw STATIC IMPORTED)
    set_target_properties(gui_glfw PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../Dependencies/lib-mingw-w64/Linux/libglfw3.a"
    )
endif (UNIX)

target_link_libraries(GUI PUBLIC
        gui_glfw
        opengl32     # OpenGL (needed by GLFW)
        gdi32        # Windows GDI
        user32       # Window and input handling
        kernel32     # Core Windows functions
        ws2_32
        winpthread
)