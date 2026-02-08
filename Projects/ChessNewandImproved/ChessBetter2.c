//
// Created by Leon on 08/02/2026.
// The Start of a new era
// Trust

#include "ChessBetter2.h"

#include <stdio.h>

#define COOL_COLOR (Vec3f){.2, .3, .3}
#define COLOR_WHITE (Vec3f){1, 1, 1}
#define COLOR_BLACK (Vec3f){0, 0, 0}
#define COLOR_GRAY (Vec3f){0.2, 0.2, 0.2}
#define COLOR_LIGHTGREY (Vec3f){0.8, 0.8, 0.8}
#define COLOR_DARKYELLOW (Vec3f){0.5, 0.5, 0.0}
#define COLOR_HOVER (Vec3f){0.8, 0.8, 1}
#define CHESS_PORT 52345



typedef enum {

    empty = 0,

    whitePawn = 1,
    whiteKnight = 2,
    whiteBishop = 3,
    whiteRook = 4,
    whiteQueen = 5,
    whiteKing = 6,

    blackPawn = -1,
    blackKnight = -2,
    blackBishop = -3,
    blackRook = -4,
    blackQueen = -5,
    blackKing = -6,

} ChessPiece;

typedef enum {

    White,
    Black,

} Color;

typedef enum {

    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6,

} PieceType;

typedef struct {

    ChessPiece piece;
    Color squareColor;

} Square;

typedef struct {

    Square squares [8][8];

} Board;

Board chess_board = {};

Color chess_getPieceColor(ChessPiece piece) {

    if (piece > 0) return White;
    else return Black;

}

void chess_setUpPieces () {

    int j = 0;
    for (int i = 0; j < 8; i++) {

        chess_board.squares[j][i].piece = empty;

        if (i == 7) {

            j++;
            i = 0;

        }

    }

    chess_board.squares[0][0].piece = blackRook;
    chess_board.squares[0][1].piece = blackKnight;
    chess_board.squares[0][2].piece = blackBishop;
    chess_board.squares[0][3].piece = blackQueen;
    chess_board.squares[0][4].piece = blackKing;
    chess_board.squares[0][5].piece = blackBishop;
    chess_board.squares[0][6].piece = blackKnight;
    chess_board.squares[0][7].piece = blackRook;

    chess_board.squares[7][0].piece = whiteRook;
    chess_board.squares[7][1].piece = whiteKnight;
    chess_board.squares[7][2].piece = whiteBishop;
    chess_board.squares[7][3].piece = whiteQueen;
    chess_board.squares[7][4].piece = whiteKing;
    chess_board.squares[7][5].piece = whiteBishop;
    chess_board.squares[7][6].piece = whiteKnight;
    chess_board.squares[7][7].piece = whiteRook;

    for (int i = 0; i < 8; i++) {

        chess_board.squares[1][i].piece = blackPawn;
        chess_board.squares[6][i].piece = whitePawn;

    }
}

void printBoard () {

    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {

            //printf("chess",);
            //board.squares[i][ii].piece;

        }
    }

}

