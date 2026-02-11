//
// Created by Leon on 08/02/2026.
// The Start of a new era
// Trust

#include "ChessBetter2.h"
#include "../../Render/GUI/GuiElement.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../Render/GUI/CallbackFunctions.h"

#define COOL_COLOR (Vec3f){.2, .3, .3}
#define COLOR_WHITE (Vec3f){1, 1, 1}
#define COLOR_BLACK (Vec3f){0, 0, 0}
#define COLOR_GRAY (Vec3f){0.2, 0.2, 0.2}
#define COLOR_LIGHTGREY (Vec3f){0.8, 0.8, 0.8}
#define COLOR_DARKYELLOW (Vec3f){0.5, 0.5, 0.0}
#define COLOR_HOVER (Vec3f){0.8, 0.8, 1}
#define CHESS_PORT 52345

void chess_loadChessPosition(char* fen);


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
    Color turn;

    Square squares [8][8];
    PossibleMove possibleMoves [64];
    int index;

} Board;


Board chess_board = {};
Simple_Texture* chess_pieceTextures[13] = {};

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

    //chess_setUpPieces ();
    chess_loadChessPosition("q3rr2/1b1p4/3P1n1k/4p1p1/1P2P3/2N1Q2P/5PP1/6K1 b - e3 17 32");
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

void chess_loadChessPosition(char* fen) {
    String fenString = stringOf(fen);
    List_String fenPieces = str_split(&fenString, " ");

    List_String ranks = str_split(String_ListGet(&fenPieces, 0), "/");
    for (int i = 0; i < ranks.size; i++) {
        String* rank = &ranks.content[i];
        int col = 0;
        for (int j = 0; j < rank->length; j++) {
            char c = rank->content[j];
            switch (c) {
                case 'p': chess_board.squares[i][col++].piece = blackPawn; break;
                case 'n': chess_board.squares[i][col++].piece = blackKing; break;
                case 'b': chess_board.squares[i][col++].piece = blackBishop; break;
                case 'r': chess_board.squares[i][col++].piece = blackRook; break;
                case 'q': chess_board.squares[i][col++].piece = blackQueen; break;
                case 'k': chess_board.squares[i][col++].piece = blackKing; break;

                case 'P': chess_board.squares[i][col++].piece = whitePawn; break;
                case 'N': chess_board.squares[i][col++].piece = whiteKing; break;
                case 'B': chess_board.squares[i][col++].piece = whiteBishop; break;
                case 'R': chess_board.squares[i][col++].piece = whiteRook; break;
                case 'Q': chess_board.squares[i][col++].piece = whiteQueen; break;
                case 'K': chess_board.squares[i][col++].piece = whiteKing; break;

                default:
                    if (c >= '0' && c <= '9') {
                        for (int a = 0; a < c - '0'; a++) {
                            chess_board.squares[i][col++].piece = empty;
                        }
                    }
                    break;
            }
        }
    }
    chess_board.turn = String_ListGet(&fenPieces, 1)->content[0] == 'b' ? Black : White;

    String_ListFree(&fenPieces);
    String_ListFree(&ranks);
}

void chess_loadTextures() {
    chess_pieceTextures[0] = newEmptyTexture(64, 64);

    chess_pieceTextures[1] = loadTextureFromPng("ChessPieces/b_pawn_1x_ns.png");
    chess_pieceTextures[2] = loadTextureFromPng("ChessPieces/b_knight_1x_ns.png");
    chess_pieceTextures[3] = loadTextureFromPng("ChessPieces/b_bishop_1x_ns.png");
    chess_pieceTextures[4] = loadTextureFromPng("ChessPieces/b_rook_1x_ns.png");
    chess_pieceTextures[5] = loadTextureFromPng("ChessPieces/b_queen_1x_ns.png");
    chess_pieceTextures[6] = loadTextureFromPng("ChessPieces/b_king_1x_ns.png");

    chess_pieceTextures[7] = loadTextureFromPng("ChessPieces/w_pawn_1x_ns.png");
    chess_pieceTextures[8] = loadTextureFromPng("ChessPieces/w_knight_1x_ns.png");
    chess_pieceTextures[9] = loadTextureFromPng("ChessPieces/w_bishop_1x_ns.png");
    chess_pieceTextures[10] = loadTextureFromPng("ChessPieces/w_rook_1x_ns.png");
    chess_pieceTextures[11] = loadTextureFromPng("ChessPieces/w_queen_1x_ns.png");
    chess_pieceTextures[12] = loadTextureFromPng("ChessPieces/w_king_1x_ns.png");
}

static Element* createChessSquares(ElementSettings es) {
    static int row = 0;
    static int column = 0;
    es.color = ((row+column) % 2 ? COLOR_GRAY : COLOR_WHITE);

    const ElementSettings pieceDisplaySettings = {
        .texture = chess_pieceTextures[0],
        .width = es.width,
        .height = es.height,
        .notSelectable = true
    };

    Element* square = createElement(es);
    Element* piece = createElement(pieceDisplaySettings);

    if (row == 7) {
        column++;
        row = 0;
    } else {
        row++;
    }
    return square;
}

void chess_createChessBoard(Element* element) {
    Element* board = addChildrenAsGridWithGenerator(
                 (ElementSettings){
                     .color = 0.5f, 0.0f, 0.3f,
                     .width = 400,
                     .height = 400,
                 },
                 (ElementSettings){
                     .color = COLOR_WHITE,
                     .onHover = defaultHoverFun,
                     .onClick = runTaskFun,
                 }, 8, 8,
                 createChessSquares
                );

    addChildElements(element,
        addChildElements(createElement((ElementSettings){
            .draggable = true,
            .pos = {200, 200},
            .color = COLOR_DARKYELLOW,
            .padding = 10, 10, 10, 10,}
            ),
            createElement((ElementSettings){
                .text = "Chess",
                .color = COLOR_DARKYELLOW,
                .padding = {10,1,10,10},
                .notSelectable = true
            }  ),
            addChildElements(
                createElement((ElementSettings){
                    .height = 600,
                    .fixedHeight = true,
                    .notSelectable = true,
                    .color = COLOR_GRAY,
                }),
                board
                ,addChildElements(
                    createElement((ElementSettings){
                        .layoutDirection = L_right
                    })
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "a"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "b"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "c"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "d"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "e"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "f"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "g"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8, .text = "h"})
                    ),
                    addChildElements(
                createElement((ElementSettings){
                        .layoutDirection = L_down,
                    })

                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "1"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "2"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "3"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "4"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "5"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "6"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "7"})
                    ,createElement((ElementSettings){.color =  0.5,0,0.25,.width = 400/8,.height = 400/8,.text = "8"})
                )
             )


             )
    );
}