//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"

#include <stdlib.h>

#include "Render/GUI/CallbackFunctions.h"
#include "Utils/Makros/Makros.h"
#include "Utils/Misc/UtilityFun.h"
#include "GuiInterface.h"
#include "Render/GUI/GuiElement.h"
#include "Render/GUI/ElementTypes/TextField.h"


#define COOL_COLOR (Vec3f){.2f, .3f, .3f}
#define COLOR_WHITE (Vec3f){1.f, 1.f, 1.f}
#define COLOR_BLACK (Vec3f){0, 0, 0}
#define COLOR_GRAY (Vec3f){0.2f, 0.2f, 0.2f}
#define COLOR_LIGHTGREY (Vec3f){0.8f, 0.8f, 0.8f}
#define COLOR_DARKYELLOW (Vec3f){0.5f, 0.5f, 0.0f}
#define COLOR_HOVER (Vec3f){0.8f, 0.8f, 1.0f}

#define CHESS_PORT 52345

typedef enum {
    PIECE_NONE = 0,

    PIECE_BLACK_PAWN = 1,
    PIECE_BLACK_KNIGHT = 2,
    PIECE_BLACK_BISHOP = 3,
    PIECE_BLACK_ROOK = 4,
    PIECE_BLACK_QUEEN = 5,
    PIECE_BLACK_KING = 6,

    PIECE_WHITE_PAWN = -1,
    PIECE_WHITE_KNIGHT = -2,
    PIECE_WHITE_BISHOP = -3,
    PIECE_WHITE_ROOK = -4,
    PIECE_WHITE_QUEEN = -5,
    PIECE_WHITE_KING = -6

} ChessPiece;

typedef struct {
    int fromRow, fromCol;
    int toRow, toCol;
    ChessPiece promotion;
} ChessMove;

typedef struct {
    ChessPiece piece;
    int isMarked;
} Square;

typedef enum {
    TYPE_NONE = 0,
    TYPE_PAWN = 1,
    TYPE_KNIGHT = 2,
    TYPE_BISHOP = 3,
    TYPE_ROOK = 4,
    TYPE_QUEEN = 5,
    TYPE_KING = 6,
} PieceType;

typedef enum {
    COL_NONE = 0,
    COL_BLACK = 1,
    COL_WHITE = -1
} PieceColor;

typedef enum {
    attack = 0,
    defend = 1
} MarkType;

typedef enum {
    whiteDown = 0,
    whiteUp = 1
} BoardDirection;

typedef void (*MarkerFun)(PieceColor color, int row, int column, MarkType type);
typedef bool (*Marker)(int row, int column, int pieceRow, int pieceCol, PieceColor color);

static Square board[8][8] = {};
static ElementHandle pieceSlots[8][8] = {};
static BoardDirection boardDirection = whiteDown;
static bool blackCanCastle = true;
static bool whiteCanCastle = true;
static bool isMultiplayer = false;

static pthread_t multiplayerListener;

static PieceColor turn = -1;

static char* pieceTextures[13] = {};

static void chess_loadTextures();

static void createStartScreen(Element* root);
static void createChessBoard(Element* root);
static void createEndScreen(Element* root);
static void setUpPieces();
static void setUpPiecesForTest();
static void switchSides(void*);
static void syncGui();
static Vec2i getPosition(const Element* element);

static bool applyMove(const ChessMove* move);

static void sendMove(const ChessMove *move);

static bool markAttack(int row, int column, int pieceRow, int pieceCol, PieceColor color);
static bool markDefend(int row, int column, int pieceRow, int pieceCol, PieceColor color);
static bool markerFun(int row, int column, int pieceRow, int pieceCol, PieceColor color, MarkType type);

static bool isKingAttacked(PieceColor color);
static bool isCheckmate(PieceColor color);
static bool doesMoveCauseCheck(int row, int column, int pieceRow, int pieceCol, PieceColor pieceColor);

static void markedOnlyIfEnemy(int row, int column, int pieceRow, int pieceCol, PieceColor color);
static void markedOnlyIfFree(int row, int column, int pieceRow, int pieceCol, PieceColor color);
static void chessCheckedMark(int row, int column, int pieceRow, int pieceCol, PieceColor color);

static void runMarking(ChessPiece piece, int row, int column, MarkType type);

static void runMarkAllPieces(PieceColor color, MarkType type);
static void unmarkAll(PieceColor color, int row, int column, MarkType type);

static PieceColor getColor(int piece) {
    return piece ? sig(piece) : COL_NONE;
}

static void markPawn(const PieceColor color, const int row, const int column, const MarkType type) {
    if (type == attack) {
        markedOnlyIfEnemy(row + color, column+1, row, column, color);
        markedOnlyIfEnemy(row + color, column-1, row, column, color);
        markedOnlyIfFree(row + color, column, row, column, color);
        if (((color == 1 && row == 1 )||(color == -1 && row == 6)) && board[row+color][column].piece == PIECE_NONE) markedOnlyIfFree(row + 2*color, column, row, column, color);
    } else {
        markDefend(row + color, column+1, row, column, color);
        markDefend(row + color, column-1, row, column, color);
    }
}

static void markKnight(const PieceColor color, const int row, const int column, const MarkType type) {
    markerFun(row+1, column+2, row, column, color, type);
    markerFun(row-1, column-2, row, column, color, type);
    markerFun(row+1, column-2, row, column, color, type);
    markerFun(row-1, column+2, row, column, color, type);
    markerFun(row+2, column+1, row, column, color, type);
    markerFun(row-2, column-1, row, column, color, type);
    markerFun(row+2, column-1, row, column, color, type);
    markerFun(row-2, column+1, row, column, color, type);
}

static void markBishop(const PieceColor color, const int row, const int column, const MarkType type) {
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

static void markRook(const PieceColor color, const int row, const int column, const MarkType type) {
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

static void markQueen(const PieceColor color, const int row, const int column, const MarkType type) {
    markBishop(color, row, column, type);
    markRook(color, row, column, type);
}

static void markKing(const PieceColor color, const int row, const int column, const MarkType type) {
    if (type == attack) {
        runMarkAllPieces(-color, defend);

        if ((color == COL_BLACK && whiteCanCastle) || (color == COL_WHITE && blackCanCastle)) {
            for (int i = 1;; i++) {
                if (column+i > 7) break;
                if (board[row][column+i].piece == PIECE_NONE) continue;
                if (board[row][column+i].piece == TYPE_ROOK * color) {
                    chessCheckedMark(row, 6, row, column, color);
                } else if (board[row][column+i].piece != PIECE_NONE) break;
            }
            for (int i = 1;; i++) {
                if (column-i < 0) break;
                if (board[row][column-i].piece == PIECE_NONE) continue;
                if (board[row][column-i].piece == TYPE_ROOK * color) {
                    chessCheckedMark(row, 2, row, column, color);
                } else if (board[row][column-i].piece != PIECE_NONE) break;
            }
        }

        chessCheckedMark(row+1, column,   row, column, color);
        chessCheckedMark(row-1, column,   row, column, color);
        chessCheckedMark(row,   column+1, row, column, color);
        chessCheckedMark(row,   column-1, row, column, color);
        chessCheckedMark(row+1, column+1, row, column, color);
        chessCheckedMark(row-1, column-1, row, column, color);
        chessCheckedMark(row+1, column-1, row, column, color);
        chessCheckedMark(row-1, column+1, row, column, color);

        unmarkAll(0,0,0,0);
    } else {
        markDefend(row+1, column,   row, column, color);
        markDefend(row-1, column,   row, column, color);
        markDefend(row,   column+1, row, column, color);
        markDefend(row,   column-1, row, column, color);
        markDefend(row+1, column+1, row, column, color);
        markDefend(row-1, column-1, row, column, color);
        markDefend(row+1, column-1, row, column, color);
        markDefend(row-1, column+1, row, column, color);
    }

}

static void runMarkAllPieces(const PieceColor color, const MarkType type) {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (getColor(board[i][ii].piece) == color) runMarking(board[i][ii].piece, i, ii, type);
        }
    }
}

static void unmarkAll(const PieceColor color, const int row, const int column, const MarkType type) {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (board[i][ii].isMarked) board[i][ii].isMarked--;
        }
    }
}

static bool markerFun(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color, const MarkType type) {
    static Marker markingFuns[] = {markAttack, markDefend};
    return markingFuns[type](row, column, pieceRow, pieceCol, color);
}

static bool markAttack(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return 1;
    if (getColor(board[row][column].piece) != color) {
        board[row][column].isMarked =  !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
    return abs(board[row][column].piece) != 0;
}

static bool markDefend(const int row, const int column, int pieceRow, int pieceCol, PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return 1;
    board[row][column].isMarked = true;
    return abs(board[row][column].piece) != 0;
}

static void chessCheckedMark(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (getColor(board[row][column].piece) != color && !board[row][column].isMarked) {
        board[row][column].isMarked = 2*!doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
}

static void markedOnlyIfEnemy(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (-getColor(board[row][column].piece) == color) {
        board[row][column].isMarked = !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
}

static void markedOnlyIfFree(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (getColor(board[row][column].piece) == 0) {
        board[row][column].isMarked = !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
}

static bool doesMoveCauseCheck(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor pieceColor) {
    int copyMarkers[8][8] = {};

    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            copyMarkers[i][ii] = board[i][ii].isMarked;
            board[i][ii].isMarked = false;
        }
    }

    const ChessPiece temp = board[row][column].piece;

    board[row][column].piece = board[pieceRow][pieceCol].piece;
    board[pieceRow][pieceCol].piece = PIECE_NONE;

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
    chess_loadTextures();
    createChessBoard(root);
    createStartScreen(root);

    createEndScreen(root);
}

char *Chess_getBoardFEN() {
    char* str = calloc(128, 1);
    int len = 0;
    bool empty = false;
    int numEmpty = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++){
            const ChessPiece piece = board[i][j].piece;
        
            if (piece == PIECE_NONE) {
                empty = true;
                numEmpty++;
            } else {
                constexpr char map[] = {
                    'P','N','B','R','Q','K',
                    'p','n','b','r','q','k'
                };
                if (empty) {
                    str[len++] = '0' + numEmpty;
                    numEmpty = 0;
                    empty = false;
                }
                const int idx = abs(piece) + ((getColor(piece) == COL_WHITE) ? -1 : 5);
                str[len++] = map[idx];
            }        
        }
        if (empty) {
            str[len++] = '0' + numEmpty;
            numEmpty = 0;
            empty = false;
        }
        str[len++] = '/';
    }
    len--;
    str[len++] = ' ';
    str[len++] = (turn == COL_WHITE) ? 'w' : 'b';
    str[len++] = ' ';
    str[len++] = '-';
    str[len++] = ' ';
    str[len++] = '-';

    return str;
}

#if 0
static void sendMove(const ChessMove *move) {
    if (!isMultiplayer) return;
    sendData(gameSocket, move, sizeof(ChessMove));
}

static void* receiveMoves(void*) {
    ChessMove receivedMove;
    receiveData(gameSocket, &receivedMove, sizeof(ChessMove));
    return NULL;
}

static void hostGame() {
    isMultiplayer = true;
    gameSocket = createServerSocket(CHESS_PORT);
    pthread_create(&multiplayerListener, nullptr, receiveMoves, NULL);
}

static void joinGame(const char* ip) {
    isMultiplayer = true;
    gameSocket = createClientSocket(ip, CHESS_PORT);
    pthread_create(&multiplayerListener, nullptr, receiveMoves, NULL);
}
#endif

static void startChessGameTask(void*) {
    Element* mainMenu = Element_getElement_ptr("start screen");
    Element* chessBoard = Element_getElement_ptr("game board");
    Element_setActive_ptr(mainMenu, false);
    Element_setActive_ptr(chessBoard, true);
}

static void showWinnerScreen(const bool winner) {
    Element* endScreen = Element_getElement_ptr("end screen");
    Element_setActive_ptr(endScreen, true);
    Element* colorDisplay = Element_getElement_ptr("color display");
    Element_setColor_ptr(colorDisplay, winner ? COLOR_WHITE : COLOR_GRAY);
    Element_setText_ptr(colorDisplay, winner ? "White won" : "Black won");
}

static bool onSquareClicked(Vec2i pos) {
    static Vec2i selectedPiecePos = {-1,-1};
    bool success = false;
    if (board[pos.y][pos.x].isMarked && selectedPiecePos.x != -1) {
        const ChessMove move = {
            .fromRow = selectedPiecePos.y,
            .fromCol = selectedPiecePos.x,
            .toRow = pos.y,
            .toCol = pos.x,
            .promotion = 0
        };
        success = applyMove(&move);
        //sendMove(&move);
    } else {
        unmarkAll(0,0,0,0);
        if (getColor(board[pos.y][pos.x].piece) == turn) {
            runMarking(board[pos.y][pos.x].piece, pos.y, pos.x, attack);
            selectedPiecePos = pos;
            success = true;
        }
    }
    syncGui();
    return success;
}

bool Chess_makeMove(int row, int col) {
    if (!onSquareClicked((Vec2i){col, row})) return false;
    return true;
}

uint64_t Chess_getMoves(int row, int col) {
    onSquareClicked((Vec2i){col, row});
    uint64_t validSquares = 0;
    for (int i = 0; i < 64; i++) {
        validSquares |= (uint64_t)board[i/8][i%8].isMarked << i;
    }
    return validSquares;
}

static void fromGuiClicked(void* el) {
    static bool isGameOver = false;
    if (isGameOver) return;
    const Element* element = (Element*) el;
    const Vec2i pos = getPosition(element);

    onSquareClicked(pos);
}

static bool applyMove(const ChessMove* move) {
    static bool isGameOver = false;
    if (isGameOver) return false;

    const Vec2i pos = {move->toCol, move->toRow};
    const Vec2i selectedPiecePos = {move->fromCol,move->fromRow};

    if (!board[pos.y][pos.x].isMarked)
        return false;

    if (blackCanCastle || whiteCanCastle) {
        if (abs(board[selectedPiecePos.y][selectedPiecePos.x].piece) == TYPE_ROOK || abs(board[selectedPiecePos.y][selectedPiecePos.x].piece) == TYPE_KING) {
            if (turn == 1) blackCanCastle = false;
            else whiteCanCastle = false;

            if (pos.x == 2) {
                if (pos.y == 0 || pos.y == 7) {
                    board[pos.y][3].piece = board[pos.y][0].piece;
                    board[pos.y][0].piece = PIECE_NONE;
                }
            } else if (pos.x == 6) {
                if (pos.y == 0 || pos.y == 7) {
                    board[pos.y][5].piece = board[pos.y][7].piece;
                    board[pos.y][7].piece = PIECE_NONE;
                }
            }
        }
    }

    board[pos.y][pos.x].piece = board[selectedPiecePos.y][selectedPiecePos.x].piece;
    board[selectedPiecePos.y][selectedPiecePos.x].piece = PIECE_NONE;
    turn *= -1;
    unmarkAll(0,0,0,0);
    if (isCheckmate(turn)) {
        showWinnerScreen(boardDirection ? turn : -turn);
        isGameOver = true;
    }

    return true;
}

static bool isKingAttacked(const PieceColor color) {
    runMarkAllPieces(color*-1, defend);
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (abs(board[i][ii].piece) == TYPE_KING && getColor(board[i][ii].piece) == color) {
                bool isKingAttacked = false;
                if (board[i][ii].isMarked) {
                    isKingAttacked = true;
                }
                unmarkAll(0,0,0,0);
                return isKingAttacked;
            }
        }
    }
    return false;
}

static bool isCheckmate(const PieceColor color) {
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
    unmarkAll(0,0,0,0);
    return checkmate;
}

static void runMarking(const ChessPiece piece, const int row, const int column, const MarkType type) {
    static MarkerFun markerFun[7] = {
        unmarkAll, markPawn, markKnight, markBishop, markRook, markQueen, markKing
    };
    markerFun[abs(piece)](getColor(piece), row, column, type);
}

static void resetBoard(void*) {
    turn = boardDirection ? 1 : -1;
    blackCanCastle = true;
    whiteCanCastle = true;
    setUpPieces();
    unmarkAll(0,0,0,0);

    Element_setActive_ptr(Element_getElement_ptr("end screen"), false);

    syncGui();
}

static Vec2i getPosition(const Element* element) {
    const Element* parent = Element_get(element->parentElement);
     return (Vec2i) {
        element->dims.pos.x / (parent->dims.width/8),
        element->dims.pos.y / (parent->dims.height/8)
    };
}

static void closeProgram(void*) {
    exit(69);
}

static char* getTextureForPiece(ChessPiece piece) {
    piece = boardDirection ? -piece : piece;
    if (piece < 0) piece = -piece + 6;
    return pieceTextures[piece];
}

static void switchSides(void*) {
    boardDirection = !boardDirection;
    turn = -turn;
    resetBoard(nullptr);
}

static void flipBoard(void*) {
    boardDirection = !boardDirection;
    turn = -turn;
    unmarkAll(0,0,0,0);
    for (int i = 0; i < 4; i++) {
        for (int ii = 0; ii < 8; ii++) {
            const ChessPiece temp = -board[i][ii].piece;
            board[i][ii].piece = -board[7-i][ii].piece;
            board[7-i][ii].piece = temp;
        }
    }
    syncGui();
}

static void setUpPieces() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            board[r][c].piece = PIECE_NONE;
        }
    }
    for (int i = 0; i < 8; i++) {
        board[1][i].piece = PIECE_BLACK_PAWN;
        board[6][i].piece = PIECE_WHITE_PAWN;
    }
    board[0][0].piece = PIECE_BLACK_ROOK;
    board[0][7].piece = PIECE_BLACK_ROOK;

    board[0][1].piece = PIECE_BLACK_KNIGHT;
    board[0][6].piece = PIECE_BLACK_KNIGHT;

    board[0][2].piece = PIECE_BLACK_BISHOP;
    board[0][5].piece = PIECE_BLACK_BISHOP;

    board[7][0].piece = PIECE_WHITE_ROOK;
    board[7][7].piece = PIECE_WHITE_ROOK;

    board[7][1].piece = PIECE_WHITE_KNIGHT;
    board[7][6].piece = PIECE_WHITE_KNIGHT;

    board[7][2].piece = PIECE_WHITE_BISHOP;
    board[7][5].piece = PIECE_WHITE_BISHOP;

    if (boardDirection == whiteDown) {
        board[7][3].piece = PIECE_WHITE_QUEEN;
        board[7][4].piece = PIECE_WHITE_KING;

        board[0][3].piece = PIECE_BLACK_QUEEN;
        board[0][4].piece = PIECE_BLACK_KING;
    } else {
        board[7][4].piece = PIECE_WHITE_QUEEN;
        board[7][3].piece = PIECE_WHITE_KING;

        board[0][4].piece = PIECE_BLACK_QUEEN;
        board[0][3].piece = PIECE_BLACK_KING;
    }
}

static void setUpPiecesForTest() {

    // Clear board first
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            board[r][c].piece = PIECE_NONE;

    // Black King
    board[0][4].piece = PIECE_BLACK_KING;   // e8

    // Black pawns (to block escape)
    board[1][5].piece = PIECE_BLACK_PAWN;   // f7
    board[1][6].piece = PIECE_BLACK_PAWN;   // g7

    // White King
    board[7][4].piece = PIECE_WHITE_KING;     // e1

    // White Queen
    board[4][7].piece = PIECE_WHITE_QUEEN;    // h4
    board[4][1].piece = PIECE_WHITE_QUEEN;
    // Set side to move: White
}

static void syncGui() {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            Element* element = Element_get(pieceSlots[i][ii]);
            gui_setTexture(element, getTextureForPiece(board[i][ii].piece));

            const Vec3f d = Element_get(element->parentElement)->visuals.defaultColor;
            const Vec3f defaultColor = {d.x,d.y,d.z};
            const Vec3f color = board[i][ii].isMarked ? Vec3f_Add(COLOR_DARKYELLOW, Vec3f_Mul(defaultColor, 0.2f)) : defaultColor;
            Element_setColor_ptr(Element_get(element->parentElement), color);
        }
    }
}


static void chess_loadTextures() {
    pieceTextures[0] = "Blank.png";

    pieceTextures[1] = "ChessPieces/b_pawn_1x_ns.png";
    pieceTextures[2] = "ChessPieces/b_knight_1x_ns.png";
    pieceTextures[3] = "ChessPieces/b_bishop_1x_ns.png";
    pieceTextures[4] = "ChessPieces/b_rook_1x_ns.png";
    pieceTextures[5] = "ChessPieces/b_queen_1x_ns.png";
    pieceTextures[6] = "ChessPieces/b_king_1x_ns.png";

    pieceTextures[7] = "ChessPieces/w_pawn_1x_ns.png";
    pieceTextures[8] = "ChessPieces/w_knight_1x_ns.png";
    pieceTextures[9] = "ChessPieces/w_bishop_1x_ns.png";
    pieceTextures[10] = "ChessPieces/w_rook_1x_ns.png";
    pieceTextures[11] = "ChessPieces/w_queen_1x_ns.png";
    pieceTextures[12] = "ChessPieces/w_king_1x_ns.png";

    gui_addTextures(
        pieceTextures[0],
        pieceTextures[1],
        pieceTextures[2],
        pieceTextures[3],
        pieceTextures[4],
        pieceTextures[5],
        pieceTextures[6],
        pieceTextures[7],
        pieceTextures[8],
        pieceTextures[9],
        pieceTextures[10],
        pieceTextures[11],
        pieceTextures[12],
        "Noise.png"
    );
}

static void closeGame(void*) {
    gui_setActive("game board", false);
    resetBoard(nullptr);
}

static ElementHandle createChessSquares(const int row, const int col, ElementSettings es) {
    es.color = (row+col) % 2 ? COLOR_GRAY : COLOR_WHITE;

    const ElementSettings pieceDisplaySettings = {
        .minWidth = es.minWidth,
        .minHeight = es.minHeight,
        .canNotBeSelected = true,
        .color = {1,1,1},
        .texture = pieceTextures[0]
    };

    const ElementHandle square = createElement(es);
    const ElementHandle piece = createElement(pieceDisplaySettings);

    pieceSlots[col][row] = piece;
    return addChildElements(Element_get(square), piece);
}

static void createChessBoard(Element* root) {
    addChildElements(root,
            Element_new((ElementSettings){
                .color = COOL_COLOR,
                .pos = {0,0},
                .posMode = POS_RELATIVE,
                .name = "game board",
                .padding = {10,10,10,10},
                .childGap = 10,
                .cornerRadius = 10,
                .draggable = true,
            },
                addChildrenAsGridWithGenerator(
                (ElementSettings){
                    .color = COLOR_DARKYELLOW,
                    .onUpdate = updateColorRainbow,
                    .minWidth = 400,
                    .minHeight = 400,
                    .padding = {10,10,10,10}
                },(ElementSettings){
                    .color = COLOR_WHITE,
                    .canBeHovered = true,
                    .onClick = runTaskFun,
                    .task = {fromGuiClicked, THIS_ELEMENT}
                },
                8, 8,createChessSquares
                ),
                Element_new((ElementSettings){
                    .layoutDirection = LAYOUT_RIGHT,
                    .childGap = 10,
                    .invisible = true,
                    .wantGrowHorizontal = true
                },
                    Element_new((ElementSettings){
                        .color = COLOR_GRAY,
                        .padding = {10,10,10,10},
                        .text = "",
                        .onUpdate = displayCurrentTime,
                        .cornerRadius = 6,
                        .flexGrow = 1.0,
                        .wantGrowVertical = true
                    }),
                    TextField_new((ElementSettings){
                        .color = COLOR_GRAY,
                        .padding = {10,10,10,10},
                        .text = "",
                        .cornerRadius = 6,
                        .flexGrow = 1.0,
                        .wantGrowVertical = true
                    },
                        nullptr
                    )
                ),
                Element_new((ElementSettings){
                    .color = {.25, .35, .355},
                    .padding = {5,5,5,5},
                    .childGap = 10,
                    .wantGrowHorizontal = true,
                    .maxWidth = 400,
                    .layoutDirection = LAYOUT_RIGHT,
                    .canNotBeSelected = true,
                    .cornerRadius = 10,
                    .name = "panel"
                },
                    Element_new((ElementSettings){
                        .color = {.2, .3, .3},
                        .text = "Reset",
                        .padding = {10, 10, 10 ,10},
                        .canBeHovered = true,
                        .onClick = runTaskFun,
                        .flexGrow = 1.0f,
                        .task = {resetBoard}
                    }),
                    Element_new((ElementSettings){
                        .color = {.6, .3, .3},
                        .text = "Switch sides",
                        .padding = {10, 10, 10 ,10},
                        .canBeHovered = true,
                        .flexGrow = 1.0f,
                        .onClick = runTaskFun,
                        .task = {switchSides},
                    }),
                    Element_new((ElementSettings){
                        .color = {.6, .3, .3},
                        .text = "Flip Board",
                        .padding = {10, 10, 10 ,10},
                        .canBeHovered = true,
                        .flexGrow = 1.0f,
                        .onClick = runTaskFun,
                        .task = {flipBoard},
                    }),
                    Element_new((ElementSettings){
                        .color = {.6, .3, .3},
                        .text = "Close",
                        .padding = {10, 10, 10 ,10},
                        .canBeHovered = true,
                        .onClick = runTaskFun,
                        .flexGrow = 1.0f,
                        .task = {closeGame},
                    })
                )
            )
    );
    gui_setActive("game board", false);
    // alt+shift = moveLine
}

static void createStartScreen(Element* root) {
    addChildElements(root,
            Element_new((ElementSettings){
                .color = COOL_COLOR,
                .posMode = POS_RELATIVE,
                .childGap = 10,
                .padding = {10,10,10,10},
                .pos = {200, 200},
                .name = "start screen",
                .cornerRadius = 10,
                .draggable = true
            },
                Element_new((ElementSettings){
                    .color = {.2, .3, .3},
                    .text = "Chess Game",
                    .canNotBeSelected = true
                }),
                Element_new((ElementSettings){
                    .layoutDirection = LAYOUT_RIGHT,
                    .wantGrowHorizontal = true,
                    .invisible = true,
                    .childGap = 10
                },
                    Element_new((ElementSettings){
                        .color = {.3, .3, .3},
                        .padding = {10,10,10,10},
                        .text = "Start",
                        .canBeHovered = true,
                        .onClick = runTaskFun,
                        .task = startChessGameTask,
                        .cornerRadius = 10,
                        .flexGrow = .75
                    }),
                    Element_new((ElementSettings){
                        .color = {.4, .0, .0},
                        .padding = {10,10,10,10},
                        .text = "End it all",
                        .canBeHovered = true,
                        .onClick = runTaskFun,
                        .task = closeProgram,
                        .cornerRadius = 10,
                        .flexGrow = 0.25
                    })
                )
            )
    );
    gui_setActive("start screen", false);
}

static void createEndScreen(Element* root) {
    addChildElements(root,
            Element_new((ElementSettings){
                .color = COOL_COLOR,
                .posMode = POS_RELATIVE,
                .childGap = 10,
                .padding = {10,10,10,10},
                .pos = {800, 200},
                .name = "end screen",
            },
                Element_new((ElementSettings){
                    .minWidth = 100,
                    .minHeight = 100,
                    .text = "test",
                    .name = "color display",
                    .padding = {10,10,10,10}
                }),
                Element_new((ElementSettings){
                    .color = {.8, .0, .0},
                    .padding = {10,10,10,10},
                    .text = "End it all",
                    .onClick = runTaskFun,
                    .task = closeProgram
                })
            )
    );
    gui_setActive("end screen", false);
}