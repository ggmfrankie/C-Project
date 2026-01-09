//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"

#include "../../Render/Engine.h"
#include "../../Render/GUI/CallbackFunctions.h"
#include "../../Utils/Makros.h"
#define COOL_COLOR (Vec3f){.2, .3, .3}
#define COLOR_WHITE (Vec3f){1, 1, 1}
#define COLOR_BLACK (Vec3f){0, 0, 0}
#define COLOR_GRAY (Vec3f){0.1, 0.1, 0.1}
#define COLOR_LIGHTGREY (Vec3f){0.8, 0.8, 0.8}
#define COLOR_DARKYELLOW (Vec3f){0.5, 0.5, 0.0}
#define COLOR_HOVER (Vec3f){0.8, 0.8, 1}

typedef enum {
    empty = 0,

    w_Pawn = 1,
    w_Knight = 2,
    w_Bishop = 3,
    w_Rook = 4,
    w_Queen = 5,
    w_King = 6,

    b_Pawn = -1,
    b_Knight = -2,
    b_Bishop = -3,
    b_Rook = -4,
    b_Queen = -5,
    b_King = -6

} ChessPiece;

typedef struct {
    ChessPiece piece;
    bool isMarked;
} Square;

typedef enum {
    null = 0,
    pawn = 1,
    knight = 2,
    bishop = 3,
    rook = 4,
    queen = 5,
    king = 6,
} PieceType;

typedef enum {
    white = 1,
    black = -1
} PieceColor;

typedef void (*MarkerFun)(PieceColor color, int row, int column);

Square board[8][8] = {};
Element* pieceSlots[8][8] = {};

Texture* pieceTextures[13] = {};

void loadTextures();

void createStartScreen(Element* root);
void createChessBoard(Element* root);
void setUpPieces();
void syncGui();
Vec2i getPosition(const Element* element);

void trySetMarked(int row, int column, PieceColor color);
void trySetMarkedOnlyIfEnemy(int row, int column, PieceColor color);
void trySetMarkedOnlyIfFree(int row, int column, PieceColor color);

void markPawn(PieceColor color, int row, int column) {
    trySetMarkedOnlyIfFree(row + color, column, color);
    trySetMarkedOnlyIfEnemy(row + color, column+1, color);
    trySetMarkedOnlyIfEnemy(row + color, column-1, color);

    if ((color == 1 && row == 1)||(color == -1 && row == 6)) {
        trySetMarked(row + 2*color, column, color);
    }
}

void markKnight(PieceColor color, int row, int column) {

}

void markBishop(PieceColor color, int row, int column) {

}

void markRook(PieceColor color, int row, int column) {

}

void markQueen(PieceColor color, int row, int column) {

}

void markKing(PieceColor color, int row, int column) {

}

void unmarkAll() {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            board[i][ii].isMarked = false;
        }
    }
}

void trySetMarked(const int row, const int column, const PieceColor color) {
    if (row > 7 || column > 7) return;
    board[row][column].isMarked = sig(board[row][column].piece) != color;
}

void trySetMarkedOnlyIfEnemy(const int row, const int column, const PieceColor color) {
    if (row > 7 || column > 7) return;
    board[row][column].isMarked = sig(board[row][column].piece)*-1 == color;
}

void trySetMarkedOnlyIfFree(const int row, const int column, const PieceColor color) {
    if (row > 7 || column > 7) return;
    board[row][column].isMarked = sig(board[row][column].piece) == 0;
}

void createChessGUI(Element* root) {
    loadTextures();
    createStartScreen(root);
    createChessBoard(root);
}

void startChessGameTask(void* nix) {
    Element* mainMenu = *Hashmap_Element_get(&g_Hashmap, "start screen");
    Element* chessBoard = *Hashmap_Element_get(&g_Hashmap, "game board");
    setVisible(mainMenu, false);
    setVisible(chessBoard, true);
}

void onSquareClicked(void* el) {
    static MarkerFun markerFun[7] = {
        unmarkAll, markPawn, markKnight, markBishop, markRook, markQueen, markKing
    };

    const Element* element = (Element*) el;
    const Vec2i pos = getPosition(element);
    printf("Clicked element at %i, %i\n", pos.x, pos.y);

    const ChessPiece currentPiece = board[pos.y][pos.x].piece;
    const PieceType piece = abs(currentPiece);
    const PieceColor color = sig(currentPiece);

    static Vec2i selectedPiecePos = {0,0};

    printf("current Piece: %i\n", piece);

    if (board[pos.y][pos.x].isMarked) {
        board[pos.y][pos.x].piece = board[selectedPiecePos.y][selectedPiecePos.x].piece;
        board[selectedPiecePos.y][selectedPiecePos.x].piece = empty;
        unmarkAll();
    } else {
        unmarkAll();
        markerFun[piece](color, pos.y, pos.x);
        selectedPiecePos = pos;
    }
    syncGui();
}

void resetBoard(void* nix) {
    setUpPieces();
    unmarkAll();
    syncGui();
}

Vec2i getPosition(const Element* element) {
    const Element* parent = element->parentElement;
     return (Vec2i) {
        element->pos.x / (parent->width/8),
        element->pos.y / (parent->height/8)
    };
}

void closeProgram(void* nix) {
    exit(69);
}

Texture* getTexture(ChessPiece piece) {
    if (piece < 0) piece = -piece + 6;
    return pieceTextures[piece];
}

void setUpPieces() {
    for (int i = 0; i < 8; i++) {
        board[1][i].piece = w_Pawn;
        board[6][i].piece = b_Pawn;
    }
    board[0][0].piece = w_Rook;
    board[0][7].piece = w_Rook;

    board[0][1].piece = w_Knight;
    board[0][6].piece = w_Knight;

    board[0][2].piece = w_Bishop;
    board[0][5].piece = w_Bishop;

    board[0][3].piece = w_Queen;
    board[0][4].piece = w_King;

    board[7][0].piece = b_Rook;
    board[7][7].piece = b_Rook;

    board[7][1].piece = b_Knight;
    board[7][6].piece = b_Knight;

    board[7][2].piece = b_Bishop;
    board[7][5].piece = b_Bishop;

    board[7][3].piece = b_Queen;
    board[7][4].piece = b_King;
}

void syncGui() {
    pthread_mutex_lock(&guiMutex);
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            pieceSlots[i][ii]->texture = getTexture(board[i][ii].piece);
            pieceSlots[i][ii]->parentElement->color = board[i][ii].isMarked ? COLOR_DARKYELLOW : pieceSlots[i][ii]->parentElement->defaultColor;
        }
    }
    pthread_mutex_unlock(&guiMutex);
}

void loadTextures() {
    pieceTextures[0] = newEmptyTexture(64, 64);

    pieceTextures[1] = loadTextureFromPng("ChessPieces/w_pawn_1x_ns.png");
    pieceTextures[2] = loadTextureFromPng("ChessPieces/w_knight_1x_ns.png");
    pieceTextures[3] = loadTextureFromPng("ChessPieces/w_bishop_1x_ns.png");
    pieceTextures[4] = loadTextureFromPng("ChessPieces/w_rook_1x_ns.png");
    pieceTextures[5] = loadTextureFromPng("ChessPieces/w_queen_1x_ns.png");
    pieceTextures[6] = loadTextureFromPng("ChessPieces/w_king_1x_ns.png");

    pieceTextures[7] = loadTextureFromPng("ChessPieces/b_pawn_1x_ns.png");
    pieceTextures[8] = loadTextureFromPng("ChessPieces/b_knight_1x_ns.png");
    pieceTextures[9] = loadTextureFromPng("ChessPieces/b_bishop_1x_ns.png");
    pieceTextures[10] = loadTextureFromPng("ChessPieces/b_rook_1x_ns.png");
    pieceTextures[11] = loadTextureFromPng("ChessPieces/b_queen_1x_ns.png");
    pieceTextures[12] = loadTextureFromPng("ChessPieces/b_king_1x_ns.png");
}

Element* createChessSquares(ElementSettings es) {
    static int row = 0;
    static int column = 0;
    es.color = (((row+column) % 2) ? COLOR_WHITE : COLOR_GRAY);

    const ElementSettings pieceDisplaySettings = {
        .texture = pieceTextures[0],
        .width = es.width,
        .height = es.height,
        .notSelectable = true
    };

    Element* square = createElement(es);
    Element* piece = createElement(pieceDisplaySettings);

    pieceSlots[row][column] = piece;

    if (row == 7) {
        column++;
        row = 0;
    } else {
        row++;
    }
    return addChildElements(square, piece);
}

void createChessBoard(Element* root) {
    static Vec3f color = COLOR_HOVER;

    addChildElements(root,
        addChildElements(
            addChildrenAsGridWithGenerator(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .posMode = POS_ABSOLUTE,
                    .height = 400,
                    .width = 400,
                    .childGap = 0,
                    .padding = (Padding){10,10,10,10},
                    .pos = (Vec2i){800, 200},
                    .name = "game board",
                    .onHover = hoverAndDragFunction
                },
                (ElementSettings){
                    .color = COLOR_WHITE,
                    .onHover = defaultHoverFunction,
                    .onClick = runTaskFunction,
                    .task = (Task){onSquareClicked, THIS_ELEMENT}
                }, 8, 8,
                createChessSquares
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.2, .3, .3},
                    .text = "Reset",
                    .padding = (Padding){10, 10, 10 ,10},
                    .onHover = defaultHoverFunction,
                    .onClick = runTaskFunction,
                    .task = (Task){resetBoard, NULL}
                }
            )
        )
    );
    Element* chessBoard = *Hashmap_Element_get(&g_Hashmap, "game board");
    chessBoard->isActive = false;
}

void createStartScreen(Element* root) {
    const Task startChessGame = (Task){startChessGameTask, NULL};
    addChildElements(root,
        addChildElements(
            createElement(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .posMode = POS_ABSOLUTE,
                    .childGap = 10,
                    .padding = (Padding){10,10,10,10},
                    .pos = (Vec2i){200, 200},
                    .name = "start screen"
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.2, .3, .3},
                    .text = "Welcome to my Chess Game"
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.3, .3, .3},
                    .padding = (Padding){10,10,10,10},
                    .text = "Start",
                    .onHover = defaultHoverFunction,
                    .onClick = runTaskFunction,
                    .task = startChessGame
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.8, .0, .0},
                    .padding = (Padding){10,10,10,10},
                    .text = "End it all",
                    .onHover = defaultHoverFunction,
                    .onClick = runTaskFunction,
                    .task = closeProgram
                }
            )
        )
    );
}