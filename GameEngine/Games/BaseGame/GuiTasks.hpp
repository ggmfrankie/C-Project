//
// Created by ertls on 02.03.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../EngineDefines.hpp"


    void Engine_changeFOV(void* FOV);
    void Engine_runCommand(void* commandString);
    void update_chessGame(void*);
    void TFE_gameCreate(void*);


#ifdef __cplusplus
    }
#endif
