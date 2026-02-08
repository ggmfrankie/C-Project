//
// Created by Leon on 08/02/2026.
// The Start of a new era
// Trust

#include "ChessBetter2.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

    int row, col;
    bool isCapture;
    bool isCheck;

} PossibleMove;

typedef struct {

    Square squares [8][8];
    PossibleMove possibleMoves [64];
    int index;

} Board;


Board chess_board = {};

Color chess_getPieceColor(ChessPiece piece) {

    if (piece > 0) return White;
    else return Black;

}

PieceType chess_getPieceType(ChessPiece piece) {

    return abs (piece);

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

void chess_printBoard () {

static char *chess_names[] = {"[ ]","[P]","[N]","[B]","[R]","[Q]","[K]"};

    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {

            printf("%s",chess_names[chess_getPieceType(chess_board.squares[i][ii].piece)]);
            if (ii == 7) printf("\n");

        }
    }

}

void chess_run () {

    chess_setUpPieces ();
    chess_printBoard ();

}

void chess_Mark (int row, int col, bool isCheck, bool isCapture) {

    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;
    chess_board.possibleMoves[chess_board.index]=(PossibleMove){.row = row,.col = col,.isCheck = isCheck,.isCapture = isCapture};

}

void chess_Checking(int row, int col,ChessPiece piece,ChessPiece ppiece) {

    if (ppiece != 0) {

        if (chess_getPieceColor(ppiece) != chess_getPieceColor(piece)) {

            bool isCheck = false;
            bool isCapture = true;

            if (chess_getPieceType(ppiece) == King) {

                isCheck = true;
                isCapture = false;

            }
            chess_Mark(row, col, isCheck, isCapture);

        }

        return;

    }

    chess_Mark(row, col, false, false);

}

void chess_generateMovesPawn (ChessPiece piece, int row, int col) {



}

void chess_generateMovesKnight (ChessPiece piece, int row, int col) {



}

void chess_generateMovesBishop (ChessPiece piece, int row, int col) {



}

void chess_generateMovesRook (ChessPiece piece, int row, int col) {

    for (int i = 1; i < 8; i++) {

        ChessPiece ppiece = chess_board.squares[row][col+i].piece;
        chess_Checking(row, col+i, ppiece, ppiece);

    }

    for (int i = 1; i < 8; i++) {

        ChessPiece ppiece = chess_board.squares[row][col-i].piece;
        chess_Checking(row, col-i, ppiece, ppiece);

    }

    for (int i = 1; i < 8; i++) {

        ChessPiece ppiece = chess_board.squares[row+i][col].piece;
        chess_Checking(row+i, col, ppiece, ppiece);

    }

    for (int i = 1; i < 8; i++) {

        ChessPiece ppiece = chess_board.squares[row-i][col].piece;
        chess_Checking(row-i, col, ppiece, ppiece);

    }
}

void chess_generateMovesQueen (ChessPiece piece, int row, int col) {



}

void chess_generateMovesKing (ChessPiece piece, int row, int col) {



}

void chess_getMoves (ChessPiece piece, int row, int col) {

    switch (chess_getPieceType(piece)) {

        case Pawn: chess_generateMovesPawn (piece, row, col); break;
        case Knight: chess_generateMovesKnight (piece, row, col); break;
        case Bishop: chess_generateMovesBishop (piece, row, col); break;
        case Rook: chess_generateMovesRook (piece, row, col); break;
        case Queen: chess_getMoves (piece, row, col); break;
        case King: chess_getMoves (piece, row, col); break;

    }

}