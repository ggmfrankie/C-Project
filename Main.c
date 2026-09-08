
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#endif


#include "C/GUI/Engine.h"
#include "Games/BaseGame/GameGui.h"
#if 1
int main(){
    Engine_loop(generateGUI);

    return 0;
}
#endif

