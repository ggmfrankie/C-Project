
#include <stdio.h>
#include <windows.h>

#include "Render/Engine.h"
#include "Utils/Map.h"
#include "Utils/CString.h"
#include "Extern/Informatik/Spannungsteiler_A3.h"
#include "Extern/Informatik/DrunkenBishop.h"
#include "Extern/Informatik/Geldautomat.h"
#include "Extern/Informatik/MonteCarlo.h"
#include "Extern/Informatik/PointerFun.h"
#include "Extern/Informatik/Namensliste.h"
#include "Extern/Informatik/TowersOfHanoi.h"
#include "Projects/Test/TestGUI.h"
#include "Projects/Chess/ChessGame.h"
#include "Projects/ChessNewandImproved/ChessBetter2.h"
#include "Projects/Tokenizer/Calculator.h"
#define WINADAPT
#ifndef WINADAPT
int main(){
    SetConsoleOutputCP(CP_UTF8);
    startEngine(chess_createChessBoard);
    //startEngine(generateTestGUI);
    //startEngine(createChessGUI);
    return 0;
}
#endif

