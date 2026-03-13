//
// Created by ertls on 02.03.2026.
//
#include "GuiTasks.hpp"

#include "GameEngine.hpp"
#include "GuiInterface.h"
#include "Render/GUI/Update.h"


extern "C" void update_setFovHighlight(void* data) {
    if (const float fov = *static_cast<float *>(data); fov == 70.0f) {
        gui_setColor("btn_fov_70", 0.3, 0.2, 0.2);
        //gui_setColor("btn_fov_90", 0.5, 1.0, 0.3);
        gui_resetColor("btn_fov_90");
    } else {
        gui_resetColor("btn_fov_70");
        gui_setColor("btn_fov_90", 0.3, 0.2, 0.2);
    }
    delete static_cast<float *>(data);
}

extern "C" void Engine_runCommand(void* commandString) {
    std::string s = {static_cast<char *>(commandString)};
    GameEngine::Get().pushTask(
        Engine::Task( [s] {
                GameEngine::Get().getCommandRegistry().run(s);
            }
        )
    );
    free(commandString);
}

extern "C" void Engine_changeFOV(void* FOV) {
    float fovValue = *static_cast<float*>(FOV);
    GameEngine::Get().pushTask(
        Engine::Task( [fovValue] {
                GameEngine::Get().getScreen().getCamera().setFOV(fovValue);
                auto* payload = new float(fovValue);
                gui_pushUpdate(update_setFovHighlight, payload);
            }
        )
    );
}

extern "C" void update_chessGame(void*) {
    gui_setActive("start screen", true);
}
