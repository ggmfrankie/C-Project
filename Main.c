
#include <stdio.h>


#include "Test.h"
#include "C/GUI/Engine.h"
#include "DataStructures/CArrayList.h"
#include "Games/BaseGame/GameGui.h"
#include "_Projects_/ExampleGUI/ExampleGUI.h"
#if 1
int main(){
    Test_run();
    //Engine_loop(ExampleGui_generate);
    Engine_loop(generateGUI);


    return 0;
}
#endif

