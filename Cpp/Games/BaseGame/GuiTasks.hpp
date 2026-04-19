//
// Created by ertls on 02.03.2026.
//

#ifndef MIXEDPROJECT_GUITASKS_HPP
#define MIXEDPROJECT_GUITASKS_HPP

#ifdef __cplusplus
extern "C" {
#endif


    void Engine_changeFOV(void* FOV);
    void Engine_runCommand(void* commandString);
    void update_chessGame(void*);
    void TFE_gameCreate(void*);


#ifdef __cplusplus
    }
#endif

#endif //MIXEDPROJECT_GUITASKS_HPP