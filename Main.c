
#include <stdio.h>
#include <windows.h>

#include "C/Render/Engine.h"
#include "C/Utils/Map.h"
#include "C/Utils/CString.h"
#include "C/Extern/Informatik/Spannungsteiler_A3.h"
#include "C/Extern/Informatik/DrunkenBishop.h"
#include "C/Extern/Informatik/Geldautomat.h"
#include "C/Extern/Informatik/MonteCarlo.h"
#include "C/Extern/Informatik/PointerFun.h"
#include "C/Extern/Informatik/Namensliste.h"
#include "C/Extern/Informatik/TowersOfHanoi.h"
#include "C/Projects/Test/TestGUI.h"
#include "C/Projects/Chess/ChessGame.h"
#include "C/Projects/ChessNewandImproved/ChessBetter2.h"
#include "C/Projects/Tokenizer/Calculator.h"
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

