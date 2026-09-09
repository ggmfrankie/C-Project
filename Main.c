
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#endif


#include "C/GUI/Engine.h"
#include "Games/BaseGame/GameGui.h"
#include "_Projects_/ExampleGUI/ExampleGUI.h"
#if 1
int main(){
    Engine_loop(ExampleGui_generate);

    return 0;
}
#endif

