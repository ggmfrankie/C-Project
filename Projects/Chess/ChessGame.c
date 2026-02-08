//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"

#include "../../Render/Engine.h"
#include "../../Render/GUI/CallbackFunctions.h"
#include "../../Utils/Makros.h"
#include "../../Utils/TimeMeasurenments.h"
#include "../../Utils/Network.h"
#include <psdk_inc/_socket_types.h>
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

typedef struct {
    int fromRow, fromCol;
    int toRow, toCol;
    ChessPiece promotion;
} ChessMove;

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
    down = -1,
    up = 1
} PieceColor;

typedef enum {
    attack = 0,
    defend = 1
} MarkType;

typedef enum {
    whiteDown = 0,
    blackDown = 1
} BoardDirection;

typedef void (*MarkerFun)(PieceColor color, int row, int column, MarkType type);
typedef bool (*Marker)(int row, int column, int pieceRow, int pieceCol, PieceColor color);

Square board[8][8] = {};
Element* pieceSlots[8][8] = {};
BoardDirection boardDirection = whiteDown;
bool turnPosCanCastle = true;
bool turnNegCanCastle = true;
bool isMultiplayer = false;

SOCKET gameSocket = INVALID_SOCKET;
pthread_t multiplayerListener;

PieceColor turn = -1;

Texture* pieceTextures[13] = {};

void loadTextures();

void createStartScreen(Element* root);
void createChessBoard(Element* root);
void createEndScreen(Element* root);
void setUpPieces();
void setUpPiecesForTest();
void switchSides();
void syncGui();
Vec2i getPosition(const Element* element);

bool applyMove(const ChessMove* move);

void sendMove(const ChessMove *move);

bool markAttack(int row, int column, int pieceRow, int pieceCol, PieceColor color);
bool markDefend(int row, int column, int pieceRow, int pieceCol, PieceColor color);
bool markerFun(int row, int column, int pieceRow, int pieceCol, PieceColor color, MarkType type);

bool isKingAttacked(PieceColor color);
bool isCheckmate(PieceColor color);
bool doesMoveCauseCheck(int row, int column, int pieceRow, int pieceCol, PieceColor pieceColor);

void markedOnlyIfEnemy(int row, int column, int pieceRow, int pieceCol, PieceColor color);
void markedOnlyIfFree(int row, int column, int pieceRow, int pieceCol, PieceColor color);
void chessCheckedMark(int row, int column, int pieceRow, int pieceCol, PieceColor color);

void runMarking(ChessPiece piece, int row, int column, MarkType type);

void runMarkAllPieces(PieceColor color, MarkType type);
void unmarkAll();

void markPawn(const PieceColor color, const int row, const int column, const MarkType type) {
    if (type == attack) {
        markedOnlyIfEnemy(row + color, column+1, row, column, color);
        markedOnlyIfEnemy(row + color, column-1, row, column, color);
        markedOnlyIfFree(row + color, column, row, column, color);
        if (((color == 1 && row == 1 )||(color == -1 && row == 6)) && board[row+color][column].piece == empty) markedOnlyIfFree(row + 2*color, column, row, column, color);
    } else {
        markDefend(row + color, column+1, row, column, color);
        markDefend(row + color, column-1, row, column, color);
    }
}

void markKnight(const PieceColor color, const int row, const int column, const MarkType type) {
    markerFun(row+1, column+2, row, column, color, type);
    markerFun(row-1, column-2, row, column, color, type);
    markerFun(row+1, column-2, row, column, color, type);
    markerFun(row-1, column+2, row, column, color, type);
    markerFun(row+2, column+1, row, column, color, type);
    markerFun(row-2, column-1, row, column, color, type);
    markerFun(row+2, column-1, row, column, color, type);
    markerFun(row-2, column+1, row, column, color, type);
}

void markBishop(const PieceColor color, const int row, const int column, const MarkType type) {
    bool upRight = true;
    bool upLeft = true;
    bool downRight = true;
    bool downLeft = true;
    for (int i = 1; i < 8; i++) {

        int validMoves = 0;

        if (downRight && markerFun(row+i, column+i, row, column, color, type) == 0) {validMoves++;} else downRight = false;
        if (downLeft && markerFun(row+i, column+-i, row, column, color, type) == 0) {validMoves++;} else downLeft = false;

        if (upRight && markerFun(row+-i, column+i, row, column, color, type) == 0) {validMoves++;} else upRight = false;
        if (upLeft && markerFun(row+-i, column+-i, row, column, color, type) == 0) {validMoves++;} else upLeft = false;

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

        if (left && markerFun(row, column+-i, row, column, color, type) == 0) {validMoves++;} else left = false;
        if (right && markerFun(row, column+i, row, column, color, type) == 0) {validMoves++;} else right = false;

        if (up && markerFun(row+-i, column, row, column, color, type) == 0) {validMoves++;} else up = false;
        if (down && markerFun(row+i, column, row, column, color, type) == 0) {validMoves++;} else down = false;

        if (validMoves == 0) return;
    }
}

void markQueen(const PieceColor color, const int row, const int column, const MarkType type) {
    markBishop(color, row, column, type);
    markRook(color, row, column, type);
}

void markKing(const PieceColor color, const int row, const int column, const MarkType type) {
    if (type == attack) {
        runMarkAllPieces(color*-1, defend);

        if ((color == down && turnNegCanCastle) || (color == up && turnPosCanCastle)) {
            for (int i = 1;; i++) {
                if (column+i > 7) break;
                if (board[row][column+i].piece == empty) continue;
                if (board[row][column+i].piece == rook * color) {
                    chessCheckedMark(row, 6, row, column, color);
                } else if (board[row][column+i].piece != empty) break;
            }
            for (int i = 1;; i++) {
                if (column-i < 0) break;
                if (board[row][column-i].piece == empty) continue;
                if (board[row][column-i].piece == rook * color) {
                    chessCheckedMark(row, 2, row, column, color);
                } else if (board[row][column-i].piece != empty) break;
            }
        }

        chessCheckedMark(row+1, column, row, column, color);
        chessCheckedMark(row-1, column, row, column, color);
        chessCheckedMark(row, column+1, row, column, color);
        chessCheckedMark(row, column-1, row, column, color);
        chessCheckedMark(row+1, column+1, row, column, color);
        chessCheckedMark(row-1, column-1, row, column, color);
        chessCheckedMark(row+1, column-1, row, column, color);
        chessCheckedMark(row-1, column+1, row, column, color);

        unmarkAll();
    } else {
        markDefend(row+1, column, row, column, color);
        markDefend(row-1, column, row, column, color);
        markDefend(row, column+1, row, column, color);
        markDefend(row, column-1, row, column, color);
        markDefend(row+1, column+1, row, column, color);
        markDefend(row-1, column-1, row, column, color);
        markDefend(row+1, column-1, row, column, color);
        markDefend(row-1, column+1, row, column, color);
    }

}

void runMarkAllPieces(const PieceColor color, const MarkType type) {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (sig(board[i][ii].piece) == color) runMarking(board[i][ii].piece, i, ii, type);
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

bool markerFun(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color, const MarkType type) {
    static Marker markingFuns[] = {markAttack, markDefend};
    return markingFuns[type](row, column, pieceRow, pieceCol, color);
}

bool markAttack(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return 1;
    if (sig(board[row][column].piece) != color) {
        board[row][column].isMarked =  !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
    return abs(board[row][column].piece) != 0;
}

bool markDefend(const int row, const int column, int pieceRow, int pieceCol, PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return 1;
    board[row][column].isMarked = true;
    return abs(board[row][column].piece) != 0;
}

void chessCheckedMark(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (sig(board[row][column].piece) != color && !board[row][column].isMarked) {
        board[row][column].isMarked = 2*!doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }

}

void markedOnlyIfEnemy(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (-sig(board[row][column].piece) == color) {
        board[row][column].isMarked = !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
}

void markedOnlyIfFree(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (sig(board[row][column].piece) == 0) {
        board[row][column].isMarked = !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
}

bool doesMoveCauseCheck(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor pieceColor) {
    int copyMarkers[8][8] = {};

    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            copyMarkers[i][ii] = board[i][ii].isMarked;
            board[i][ii].isMarked = false;
        }
    }

    const ChessPiece temp = board[row][column].piece;

    board[row][column].piece = board[pieceRow][pieceCol].piece;
    board[pieceRow][pieceCol].piece = empty;

    const bool isCheck = isKingAttacked(pieceColor);

    board[pieceRow][pieceCol].piece = board[row][column].piece;
    board[row][column].piece = temp;

    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            board[i][ii].isMarked = copyMarkers[i][ii];
        }
    }

    return isCheck;
}

void createChessGUI(Element* root) {
    loadTextures();
    createChessBoard(root);
    createStartScreen(root);

    createEndScreen(root);
}

void sendMove(const ChessMove *move) {
    if (!isMultiplayer) return;
    sendData(gameSocket, move, sizeof(ChessMove));
}

void* receiveMoves() {
    ChessMove receivedMove;
    receiveData(gameSocket, &receivedMove, sizeof(ChessMove));
    return NULL;
}

void hostGame() {
    isMultiplayer = true;
    gameSocket = createServerSocket(CHESS_PORT);
    pthread_create(&multiplayerListener, NULL, receiveMoves, NULL);
}

void joinGame(const char* ip) {
    isMultiplayer = true;
    gameSocket = createClientSocket(ip, CHESS_PORT);
    pthread_create(&multiplayerListener, NULL, receiveMoves, NULL);
}

void startChessGameTask(void* nix) {
    Element* mainMenu = getElement("start screen");
    Element* chessBoard = getElement("game board");
    setVisible(mainMenu, false);
    setVisible(chessBoard, true);
}

void showWinnerScreen(const bool winner) {
    Element* endScreen = getElement("end screen");
    setVisible(endScreen, true);
    Element* colorDisplay = getElement("color display");
    setColor(colorDisplay, winner ? COLOR_WHITE : COLOR_GRAY);
    setText(colorDisplay, winner ? "White won" : "Black won");
}

/*
void onSquareClicked(void* el) {
    static bool isGameOver = false;
    if (isGameOver) return;
    const Element* element = (Element*) el;
    const Vec2i pos = getPosition(element);

    const ChessPiece currentPiece = board[pos.y][pos.x].piece;

    static Vec2i selectedPiecePos = {0,0};

    if (board[pos.y][pos.x].isMarked) {
        if (turnPosCanCastle || turnNegCanCastle) {
            if (abs(board[selectedPiecePos.y][selectedPiecePos.x].piece) == rook || abs(board[selectedPiecePos.y][selectedPiecePos.x].piece) == king) {
                if (turn == 1) {
                    turnPosCanCastle = false;
                } else {
                    turnNegCanCastle = false;
                }
                if (pos.x == 1) {
                    if (pos.y == 0 || pos.y == 7) {
                        board[pos.y][2].piece = board[pos.y][0].piece;
                        board[pos.y][0].piece = empty;
                    }
                } else if (pos.x == 6) {
                    if (pos.y == 0 || pos.y == 7) {
                        board[pos.y][5].piece = board[pos.y][7].piece;
                        board[pos.y][7].piece = empty;
                    }
                }
            }
        }

        board[pos.y][pos.x].piece = board[selectedPiecePos.y][selectedPiecePos.x].piece;
        board[selectedPiecePos.y][selectedPiecePos.x].piece = empty;
        turn *= -1;
        unmarkAll();
        if (isCheckmate(turn)) {
            showWinnerScreen(boardDirection ? turn : -turn);
            isGameOver = true;
        }
    } else {
        const u_int64 startTime = now_ns();
        unmarkAll();
        if (sig(currentPiece) == turn) {
            runMarking(currentPiece, pos.y, pos.x, attack);
        }
        selectedPiecePos = pos;
        //printf("Elapsed time: %llu\n", now_ns()-startTime);
    }
    syncGui();
}
*/

void onSquareClicked2(void* el) {
    static bool isGameOver = false;
    if (isGameOver) return;
    const Element* element = (Element*) el;
    const Vec2i pos = getPosition(element);

    static Vec2i selectedPiecePos = {-1,-1};

    if (board[pos.y][pos.x].isMarked && selectedPiecePos.x != -1) {
        const ChessMove move = {
            .fromRow = selectedPiecePos.y,
            .fromCol = selectedPiecePos.x,
            .toRow = pos.y,
            .toCol = pos.x,
            .promotion = 0
        };
        applyMove(&move);
        sendMove(&move);
    } else {
        unmarkAll();
        if (sig(board[pos.y][pos.x].piece) == turn) {
            runMarking(board[pos.y][pos.x].piece, pos.y, pos.x, attack);
            selectedPiecePos = pos;
        }
    }
    syncGui();
}

bool applyMove(const ChessMove* move) {
    static bool isGameOver = false;
    if (isGameOver) return false;

    const Vec2i pos = (Vec2i){move->toCol, move->toRow};
    const Vec2i selectedPiecePos = {move->fromCol,move->fromRow};

    if (!board[pos.y][pos.x].isMarked)
        return false;

    if (turnPosCanCastle || turnNegCanCastle) {
        if (abs(board[selectedPiecePos.y][selectedPiecePos.x].piece) == rook || abs(board[selectedPiecePos.y][selectedPiecePos.x].piece) == king) {
            if (turn == 1) turnPosCanCastle = false;
            else turnNegCanCastle = false;

            if (pos.x == 2) {
                if (pos.y == 0 || pos.y == 7) {
                    board[pos.y][3].piece = board[pos.y][0].piece;
                    board[pos.y][0].piece = empty;
                }
            } else if (pos.x == 6) {
                if (pos.y == 0 || pos.y == 7) {
                    board[pos.y][5].piece = board[pos.y][7].piece;
                    board[pos.y][7].piece = empty;
                }
            }
        }
    }

    board[pos.y][pos.x].piece = board[selectedPiecePos.y][selectedPiecePos.x].piece;
    board[selectedPiecePos.y][selectedPiecePos.x].piece = empty;
    turn *= -1;
    unmarkAll();
    if (isCheckmate(turn)) {
        showWinnerScreen(boardDirection ? turn : -turn);
        isGameOver = true;
    }

    return true;
}

bool isKingAttacked(const PieceColor color) {
    runMarkAllPieces(color*-1, defend);
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (abs(board[i][ii].piece) == king && sig(board[i][ii].piece) == color) {
                bool isKingAttacked = false;
                if (board[i][ii].isMarked) {
                    isKingAttacked = true;
                }
                unmarkAll();
                return isKingAttacked;
            }
        }
    }
    return false;
}

bool isCheckmate(const PieceColor color) {
    bool checkmate = true;
    if (!isKingAttacked(color)) return false;
    runMarkAllPieces(color, attack);
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (board[i][ii].isMarked) {
                checkmate = false;
                goto End;
            }
        }
    }
    End:
    unmarkAll();
    return checkmate;
}

void runMarking(const ChessPiece piece, const int row, const int column, const MarkType type) {
    static MarkerFun markerFun[7] = {
        unmarkAll, markPawn, markKnight, markBishop, markRook, markQueen, markKing
    };
    markerFun[abs(piece)](sig(piece), row, column, type);
}

void resetBoard(void* nix) {
    turn = boardDirection ? 1 : -1;
    turnPosCanCastle = true;
    turnNegCanCastle = true;
    setUpPieces();
    unmarkAll();
    setVisible(getElement("end screen"), false);
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
    piece = boardDirection ? -piece : piece;
    if (piece < 0) piece = -piece + 6;
    return pieceTextures[piece];
}

void switchSides() {
    boardDirection = !boardDirection;
    turn = -turn;
    resetBoard(NULL);
}

void flipBoard() {
    boardDirection = !boardDirection;
    turn = -turn;
    unmarkAll();
    for (int i = 0; i < 4; i++) {
        for (int ii = 0; ii < 8; ii++) {
            const ChessPiece temp = -board[i][ii].piece;
            board[i][ii].piece = -board[7-i][ii].piece;
            board[7-i][ii].piece = temp;
        }
    }
    syncGui();
}

void setUpPieces() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            board[r][c].piece = empty;
        }
    }
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

    board[7][0].piece = up_Rook;
    board[7][7].piece = up_Rook;

    board[7][1].piece = up_Knight;
    board[7][6].piece = up_Knight;

    board[7][2].piece = up_Bishop;
    board[7][5].piece = up_Bishop;

    if (boardDirection == whiteDown) {
        board[7][3].piece = up_Queen;
        board[7][4].piece = up_King;

        board[0][3].piece = down_Queen;
        board[0][4].piece = down_King;
    } else {
        board[7][4].piece = up_Queen;
        board[7][3].piece = up_King;

        board[0][4].piece = down_Queen;
        board[0][3].piece = down_King;
    }
}

void setUpPiecesForTest() {

    // Clear board first
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            board[r][c].piece = empty;

    // Black King
    board[0][4].piece = down_King;   // e8

    // Black pawns (to block escape)
    board[1][5].piece = down_Pawn;   // f7
    board[1][6].piece = down_Pawn;   // g7

    // White King
    board[7][4].piece = up_King;     // e1

    // White Queen
    board[4][7].piece = up_Queen;    // h4
    board[4][1].piece = up_Queen;
    // Set side to move: White
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
    es.color = ((row+column) % 2 ? COLOR_GRAY : COLOR_WHITE);

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
            createElement(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .pos = {0,0},
                    .draggable = true,
                    .posMode = POS_ABSOLUTE,
                    .name = "game board",
                    .padding = (Padding){10,10,10,10},
                    .childGap = 10
                }
            ),
            addChildrenAsGridWithGenerator(
                (ElementSettings){
                    .color = COLOR_DARKYELLOW,
                    .onUpdate = updateColorRainbow,
                    .width = 400,
                    .height = 400,
                    .padding = {10,10,10,10}
                },
                (ElementSettings){
                    .color = COLOR_WHITE,
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = (Task){onSquareClicked2, THIS_ELEMENT}
                }, 8, 8,
                createChessSquares
            ),
            addChildElements(
                createElement(
                    (ElementSettings){
                        .color = COOL_COLOR,
                        .layoutDirection = L_right,
                        .childGap = 10
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = COLOR_GRAY,
                        .padding = {10,10,10,10},
                        .text = "",
                        .onUpdate = displayCurrentTime
                    }
                )
            ),
            addChildElements(
                 createElement(
                    (ElementSettings){
                        .color = (Vec3f){.25, .35, .355},
                        .padding = {5,5,5,5},
                        .childGap = 10,
                        .width = 400,
                        .fixedWidth = true,
                        .layoutDirection = L_right,
                        .notSelectable = true,
                        .wantGrowHorizontal = true
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = (Vec3f){.2, .3, .3},
                        .text = "Reset",
                        .padding = (Padding){10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = (Task){resetBoard, NULL}
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = (Vec3f){.6, .3, .3},
                        .text = "Switch sides",
                        .padding = (Padding){10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = (Task){switchSides, NULL},
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = (Vec3f){.6, .3, .3},
                        .text = "Flip Board",
                        .padding = (Padding){10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = (Task){flipBoard, NULL},
                    }
                )
            )
        )
    );
    Element* chessBoard = getElement("game board");;
    chessBoard->flags.bits.isActive = false;
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
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = startChessGame
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.8, .0, .0},
                    .padding = (Padding){10,10,10,10},
                    .text = "End it all",
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = closeProgram
                }
            )
        )
    );
}

void createEndScreen(Element* root) {
    addChildElements(root,
        addChildElements(
            createElement(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .posMode = POS_ABSOLUTE,
                    .childGap = 10,
                    .padding = (Padding){10,10,10,10},
                    .pos = (Vec2i){800, 200},
                    .name = "end screen",
                }
            ),
            createElement(
                (ElementSettings){
                    .width = 100,
                    .height = 100,
                    .text = "test",
                    .name = "color display",
                    .padding = {10,10,10,10}
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.8, .0, .0},
                    .padding = (Padding){10,10,10,10},
                    .text = "End it all",
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = closeProgram
                }
            )
        )
    );
    Element* endScreen = getElement("end screen");
    endScreen->flags.bits.isActive = false;
}