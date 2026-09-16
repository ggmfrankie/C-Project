
#include <stdio.h>


#include "GuiInterface.h"
#include "../Cpp/Games/BaseGame/GameGui.h"
#include "Utils/Test.h"
#include "_Projects_/ExampleGUI/ExampleGUI.h"
#if 1
int main(){
    //Test_run();

    Engine_loop(ExampleGui_generate_1);
    //Engine_loop(generateGUI);


    return 0;
}
#endif