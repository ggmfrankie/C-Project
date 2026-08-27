//
// Created by Stefan on 07.01.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

struct Element;

void createChessGUI(struct Element *root);
char* Chess_getBoardFEN();

bool Chess_makeMove(int row, int col);

uint64_t Chess_getMoves(int row, int col);

#ifdef __cplusplus
    }
#endif

