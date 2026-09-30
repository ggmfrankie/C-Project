
#include <stdio.h>


#include "GuiInterface.h"
#include "../GameEngine/Games/BaseGame/GameGui.h"
#include "Utils/Test.h"
#include "Utils/DataStructures/CHashTable.h"
#include "Utils/Json/CJson.h"
#include "Utils/Logging/Logging.h"
#include "Utils/DataStructures/CString.h"
#include "_Projects_/Chess/ChessGame.h"
#include "_Projects_/ExampleGUI/ExampleGUI.h"
#if 1
int main(){
    //Test_run();

    //Engine_loop(createChessGUI);
    Engine_loop(generateGUI);
    //Engine_loop(ExampleGui_generate_1);
    //CHashTable_test();
    //String_test();

    return 0;
}
#endif