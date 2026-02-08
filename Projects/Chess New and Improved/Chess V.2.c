//
// Created by Leon on 08/02/2026.
// The Start of a new era
// Trust

#include "Chess V.2.h"

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

Board board = {};

Color getPieceColor(ChessPiece piece) {

    if (piece > 0) return White;
    else return Black;

}

void setUpPieces () {

    int j = 0;
    for (int i = 0; j < 8; i++) {

        board.squares[j][i].piece = empty;

        if (i == 7) {

            j++;
            i = 0;

        }

    }

    board.squares[0][0].piece = blackRook;
    board.squares[0][1].piece = blackKnight;
    board.squares[0][2].piece = blackBishop;
    board.squares[0][3].piece = blackQueen;
    board.squares[0][4].piece = blackKing;
    board.squares[0][5].piece = blackBishop;
    board.squares[0][6].piece = blackKnight;
    board.squares[0][7].piece = blackRook;

    board.squares[7][0].piece = whiteRook;
    board.squares[7][1].piece = whiteKnight;
    board.squares[7][2].piece = whiteBishop;
    board.squares[7][3].piece = whiteQueen;
    board.squares[7][4].piece = whiteKing;
    board.squares[7][5].piece = whiteBishop;
    board.squares[7][6].piece = whiteKnight;
    board.squares[7][7].piece = whiteRook;

    for (int i = 0; i < 8; i++) {

        board.squares[1][i].piece = blackPawn;
        board.squares[6][i].piece = whitePawn;

    }
}


