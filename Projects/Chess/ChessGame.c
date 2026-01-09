//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"

#include "../../Extern/Libraries/WinAdapt.h"
#include "../../Render/Engine.h"
#include "../../Render/GUI/CallbackFunctions.h"
#include "../../Utils/Makros.h"
#define COOL_COLOR (Vec3f){.2, .3, .3}
#define COLOR_WHITE (Vec3f){1, 1, 1}
#define COLOR_BLACK (Vec3f){0, 0, 0}
#define COLOR_GRAY (Vec3f){0.2, 0.2, 0.2}
#define COLOR_LIGHTGREY (Vec3f){0.8, 0.8, 0.8}
#define COLOR_DARKYELLOW (Vec3f){0.5, 0.5, 0.0}
#define COLOR_HOVER (Vec3f){0.8, 0.8, 1}

typedef enum {
    empty = 0,

    down_Pawn = 1,
    down_Knight = 2,
    down_Bishop = 3,
    down_Rook = 4,
    down_Queen = 5,
    down_King = 6,

    up_Pawn = -1,
    up_Knight = -2,
    up_Bishop = -3,
    up_Rook = -4,
    up_Queen = -5,
    up_King = -6

} ChessPiece;

typedef struct {
    ChessPiece piece;
    int isMarked;
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

typedef enum {
    attack = 0,
    defend = 1
} MarkType;

typedef void (*MarkerFun)(PieceColor color, int row, int column, MarkType type);
typedef bool (*Marker)(int row, int column, PieceColor color);

Square board[8][8] = {};
Element* pieceSlots[8][8] = {};

Texture* pieceTextures[13] = {};

void loadTextures();

void createStartScreen(Element* root);
void createChessBoard(Element* root);
void setUpPieces();
void syncGui();
Vec2i getPosition(const Element* element);

bool markAttack(int row, int column, PieceColor color);

bool markDefend(int row, int column, PieceColor color);

bool markerFun(int row, int column, PieceColor color, MarkType type);

void markedOnlyIfEnemy(int row, int column, PieceColor color);
void markedOnlyIfFree(int row, int column);
void chessCheckedMark(int row, int column, PieceColor color);

void runMarking(ChessPiece piece, int row, int column, MarkType type);
void runMarkAllPieces(PieceColor color);
void unmarkAll();

void markPawn(const PieceColor color, const int row, const int column, const MarkType type) {

    if (type == attack) {
        markedOnlyIfEnemy(row + color, column+1, color);
        markedOnlyIfEnemy(row + color, column-1, color);
        markedOnlyIfFree(row + color, column);
        if ((color == 1 && row == 1)||(color == -1 && row == 6)) markedOnlyIfFree(row + 2*color, column);
    } else {
        markDefend(row + color, column+1, color);
        markDefend(row + color, column-1, color);
    }
}

void markKnight(const PieceColor color, const int row, const int column, const MarkType type) {
    markerFun(row+1, column+2, color, type);
    markerFun(row-1, column-2, color, type);
    markerFun(row+1, column-2, color, type);
    markerFun(row-1, column+2, color, type);
    markerFun(row+2, column+1, color, type);
    markerFun(row-2, column-1, color, type);
    markerFun(row+2, column-1, color, type);
    markerFun(row-2, column+1, color, type);
}

void markBishop(const PieceColor color, const int row, const int column, const MarkType type) {
    bool upRight = true;
    bool upLeft = true;
    bool downRight = true;
    bool downLeft = true;
    for (int i = 1; i < 8; i++) {

        int validMoves = 0;

        if (downRight && markerFun(row+i, column+i, color, type) == 0) {validMoves++;} else downRight = false;
        if (downLeft && markerFun(row+i, column+i*-1, color, type) == 0) {validMoves++;} else downLeft = false;

        if (upRight && markerFun(row+i*-1, column+i, color, type) == 0) {validMoves++;} else upRight = false;
        if (upLeft && markerFun(row+i*-1, column+i*-1, color, type) == 0) {validMoves++;} else upLeft = false;

        if (validMoves == 0) return;
    }
}

void markRook(const PieceColor color, const int row, const int column, const MarkType type) {
    bool up = true;
    bool down = true;
    bool left = true;
    bool right = true;
    for (int i = 1; i < 8; i++) {

        int validMoves = 0;

        if (left && markerFun(row, column+i*-1, color, type) == 0) {validMoves++;} else left = false;
        if (right && markerFun(row, column+i, color, type) == 0) {validMoves++;} else right = false;

        if (up && markerFun(row+i*-1, column, color, type) == 0) {validMoves++;} else up = false;
        if (down && markerFun(row+i, column, color, type) == 0) {validMoves++;} else down = false;

        if (validMoves == 0) return;
    }
}

void markQueen(const PieceColor color, const int row, const int column, const MarkType type) {
    markBishop(color, row, column, type);
    markRook(color, row, column, type);
}

void markKing(const PieceColor color, const int row, const int column, const MarkType type) {
    if (type == attack) {
        runMarkAllPieces(color*-1);

        chessCheckedMark(row+1, column, color);
        chessCheckedMark(row-1, column, color);
        chessCheckedMark(row, column+1, color);
        chessCheckedMark(row, column-1, color);
        chessCheckedMark(row+1, column+1, color);
        chessCheckedMark(row-1, column-1, color);
        chessCheckedMark(row+1, column-1, color);
        chessCheckedMark(row-1, column+1, color);

        unmarkAll();
    } else {
        markDefend(row+1, column, color);
        markDefend(row-1, column, color);
        markDefend(row, column+1, color);
        markDefend(row, column-1, color);
        markDefend(row+1, column+1, color);
        markDefend(row-1, column-1, color);
        markDefend(row+1, column-1, color);
        markDefend(row-1, column+1, color);
    }

}

void runMarkAllPieces(const PieceColor color) {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (sig(board[i][ii].piece) == color) runMarking(board[i][ii].piece, i, ii, defend);
        }
    }
}

void unmarkAll() {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (board[i][ii].isMarked) board[i][ii].isMarked--;
        }
    }
}

bool markerFun(const int row, const int column, const PieceColor color, const MarkType type) {
    static Marker markingFuns[] = {markAttack, markDefend};
    return markingFuns[type](row, column, color);
}

bool markAttack(const int row, const int column, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return 1;
    board[row][column].isMarked = sig(board[row][column].piece) != color;
    return abs(board[row][column].piece) != 0;
}

bool markDefend(const int row, const int column, PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return 1;
    board[row][column].isMarked = true;
    return abs(board[row][column].piece) != 0;
}

void chessCheckedMark(const int row, const int column, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    board[row][column].isMarked = 2*(sig(board[row][column].piece) != color && !board[row][column].isMarked);
}

void markedOnlyIfEnemy(const int row, const int column, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    board[row][column].isMarked = sig(board[row][column].piece)*-1 == color;
}

void markedOnlyIfFree(const int row, const int column) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    board[row][column].isMarked = sig(board[row][column].piece) == 0;
}

void createChessGUI(Element* root) {
    loadTextures();
    createStartScreen(root);
    createChessBoard(root);
}

void startChessGameTask(void* nix) {
    Element* mainMenu = getElement("start screen");
    Element* chessBoard = getElement("game board");
    setVisible(mainMenu, false);
    setVisible(chessBoard, true);
}

void onSquareClicked(void* el) {

    const Element* element = (Element*) el;
    const Vec2i pos = getPosition(element);

    const ChessPiece currentPiece = board[pos.y][pos.x].piece;

    static Vec2i selectedPiecePos = {0,0};
    static int turn = -1;

    if (board[pos.y][pos.x].isMarked) {
        board[pos.y][pos.x].piece = board[selectedPiecePos.y][selectedPiecePos.x].piece;
        board[selectedPiecePos.y][selectedPiecePos.x].piece = empty;
        turn *= -1;
        unmarkAll();
    } else {
        unmarkAll();
        if (sig(currentPiece) == turn) {
            runMarking(currentPiece, pos.y, pos.x, attack);
        }
        selectedPiecePos = pos;
    }
    syncGui();
}

void runMarking(const ChessPiece piece, const int row, const int column, const MarkType type) {
    static MarkerFun markerFun[7] = {
        unmarkAll, markPawn, markKnight, markBishop, markRook, markQueen, markKing
    };
    markerFun[abs(piece)](sig(piece), row, column, type);
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
        board[1][i].piece = down_Pawn;
        board[6][i].piece = up_Pawn;
    }
    board[0][0].piece = down_Rook;
    board[0][7].piece = down_Rook;

    board[0][1].piece = down_Knight;
    board[0][6].piece = down_Knight;

    board[0][2].piece = down_Bishop;
    board[0][5].piece = down_Bishop;

    board[0][3].piece = down_Queen;
    board[0][4].piece = down_King;

    board[7][0].piece = up_Rook;
    board[7][7].piece = up_Rook;

    board[7][1].piece = up_Knight;
    board[7][6].piece = up_Knight;

    board[7][2].piece = up_Bishop;
    board[7][5].piece = up_Bishop;

    board[7][3].piece = up_Queen;
    board[7][4].piece = up_King;
}

void syncGui() {
    pthread_mutex_lock(&guiMutex);
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            pieceSlots[i][ii]->texture = getTexture(board[i][ii].piece);
            pieceSlots[i][ii]->parentElement->color = board[i][ii].isMarked ? Vec3f_Add(COLOR_DARKYELLOW, Vec3f_Mul(pieceSlots[i][ii]->parentElement->defaultColor, 0.2f)) : pieceSlots[i][ii]->parentElement->defaultColor;
        }
    }
    pthread_mutex_unlock(&guiMutex);
}

void loadTextures() {
    pieceTextures[0] = newEmptyTexture(64, 64);

    pieceTextures[1] = loadTextureFromPng("ChessPieces/b_pawn_1x_ns.png");
    pieceTextures[2] = loadTextureFromPng("ChessPieces/b_knight_1x_ns.png");
    pieceTextures[3] = loadTextureFromPng("ChessPieces/b_bishop_1x_ns.png");
    pieceTextures[4] = loadTextureFromPng("ChessPieces/b_rook_1x_ns.png");
    pieceTextures[5] = loadTextureFromPng("ChessPieces/b_queen_1x_ns.png");
    pieceTextures[6] = loadTextureFromPng("ChessPieces/b_king_1x_ns.png");

    pieceTextures[7] = loadTextureFromPng("ChessPieces/w_pawn_1x_ns.png");
    pieceTextures[8] = loadTextureFromPng("ChessPieces/w_knight_1x_ns.png");
    pieceTextures[9] = loadTextureFromPng("ChessPieces/w_bishop_1x_ns.png");
    pieceTextures[10] = loadTextureFromPng("ChessPieces/w_rook_1x_ns.png");
    pieceTextures[11] = loadTextureFromPng("ChessPieces/w_queen_1x_ns.png");
    pieceTextures[12] = loadTextureFromPng("ChessPieces/w_king_1x_ns.png");
}

Element* createChessSquares(ElementSettings es) {
    static int row = 0;
    static int column = 0;
    es.color = (((row+column) % 2) ? COLOR_GRAY : COLOR_WHITE);

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
    Element* chessBoard = getElement("game board");;
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