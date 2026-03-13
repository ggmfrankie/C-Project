//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"

#include "../../Render/Engine.h"
#include "../../Render/GUI/CallbackFunctions.h"
#include "../../Utils/Makros.h"
#include "../../Utils/Network.h"
#include <psdk_inc/_socket_types.h>

#include "GuiInterface.h"
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

static Square board[8][8] = {};
static Element* pieceSlots[8][8] = {};
static BoardDirection boardDirection = whiteDown;
static bool turnPosCanCastle = true;
static bool turnNegCanCastle = true;
static bool isMultiplayer = false;

static SOCKET gameSocket = INVALID_SOCKET;
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

static void markPawn(const PieceColor color, const int row, const int column, const MarkType type) {
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

        unmarkAll(0,0,0,0);
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

static void runMarkAllPieces(const PieceColor color, const MarkType type) {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {
            if (sig(board[i][ii].piece) == color) runMarking(board[i][ii].piece, i, ii, type);
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
    if (sig(board[row][column].piece) != color) {
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
    if (sig(board[row][column].piece) != color && !board[row][column].isMarked) {
        board[row][column].isMarked = 2*!doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }

}

static void markedOnlyIfEnemy(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (-sig(board[row][column].piece) == color) {
        board[row][column].isMarked = !doesMoveCauseCheck(row, column, pieceRow, pieceCol, color);
    }
}

static void markedOnlyIfFree(const int row, const int column, const int pieceRow, const int pieceCol, const PieceColor color) {
    if (row > 7 || column > 7 || row < 0 || column < 0) return;
    if (sig(board[row][column].piece) == 0) {
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
    chess_loadTextures();
    createChessBoard(root);
    createStartScreen(root);

    createEndScreen(root);
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

static void startChessGameTask(void* nix) {
    Element* mainMenu = getElement("start screen");
    Element* chessBoard = getElement("game board");
    setActive(mainMenu, false);
    setActive(chessBoard, true);
}

static void showWinnerScreen(const bool winner) {
    Element* endScreen = getElement("end screen");
    setActive(endScreen, true);
    Element* colorDisplay = getElement("color display");
    Element_setColor(colorDisplay, winner ? COLOR_WHITE : COLOR_GRAY);
    setText(colorDisplay, winner ? "White won" : "Black won");
}

static void onSquareClicked2(void* el) {
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
        //sendMove(&move);
    } else {
        unmarkAll(0,0,0,0);
        if (sig(board[pos.y][pos.x].piece) == turn) {
            runMarking(board[pos.y][pos.x].piece, pos.y, pos.x, attack);
            selectedPiecePos = pos;
        }
    }
    syncGui();
}

static bool applyMove(const ChessMove* move) {
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
            if (abs(board[i][ii].piece) == king && sig(board[i][ii].piece) == color) {
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
    markerFun[abs(piece)](sig(piece), row, column, type);
}

static void resetBoard(void* nix) {

    turn = boardDirection ? 1 : -1;
    turnPosCanCastle = true;
    turnNegCanCastle = true;
    setUpPieces();
    unmarkAll(0,0,0,0);

    setActive(getElement("end screen"), false);

    syncGui();

}

static Vec2i getPosition(const Element* element) {
    const Element* parent = element->parentElement;
     return (Vec2i) {
        element->dims.pos.x / (parent->dims.width/8),
        element->dims.pos.y / (parent->dims.height/8)
    };
}

static void closeProgram(void* nix) {
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
    resetBoard(NULL);
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

static void setUpPiecesForTest() {

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

static void syncGui() {
    for (int i = 0; i < 8; i++) {
        for (int ii = 0; ii < 8; ii++) {

            gui_setTexture(pieceSlots[i][ii], getTextureForPiece(board[i][ii].piece));

            Vec3f d = pieceSlots[i][ii]->parentElement->visuals.defaultColor;
            Vec3f defaultColor = (Vec3f){d.x,d.y,d.z};
            Vec3f color = board[i][ii].isMarked ? Vec3f_Add(COLOR_DARKYELLOW, Vec3f_Mul(defaultColor, 0.2f)) : defaultColor;
            Element_setColor(pieceSlots[i][ii]->parentElement, color);
            }
    }
}


static void chess_loadTextures() {
    pieceTextures[0] = nullptr;

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

    gui_loadTextures(
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
        pieceTextures[12]
    );
}

static void closeGame(void*) {
    gui_setActive("game board", false);
    resetBoard(nullptr);
}

static Element* createChessSquares(const int row, const int col, ElementSettings es) {
    es.color = ((row+col) % 2 ? COLOR_GRAY : COLOR_WHITE);

    const ElementSettings pieceDisplaySettings = {
        .width = es.width,
        .height = es.height,
        .notSelectable = true,
        .color = {},
        .transparency = 1.0f,
    };

    Element* square = createElement(es);
    Element* piece = createElement(pieceDisplaySettings);

    pieceSlots[col][row] = piece;
    return addChildElements(square, piece);
}

static void createChessBoard(Element* root) {
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
                    .childGap = 10,
                    .cornerRadius = 10
                }
            ),
            addChildrenAsGridWithGenerator(
                (ElementSettings){
                    .color = COLOR_DARKYELLOW,
                    .onUpdate = updateColorRainbow,
                    .width = 400,
                    .height = 400,
                    .fixedWidth = true,
                    .fixedHeight = true,
                    .padding = {10,10,10,10}
                },
                (ElementSettings){
                    .color = COLOR_WHITE,
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = {onSquareClicked2, THIS_ELEMENT}
                }, 8, 8,
                createChessSquares
            ),
            addChildElements(
                createElement(
                    (ElementSettings){
                        .color = COOL_COLOR,
                        .layoutDirection = L_right,
                        .childGap = 10,
                        .cornerRadius = 10
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
                        .color = {.25, .35, .355},
                        .padding = {5,5,5,5},
                        .childGap = 10,
                        .width = 400,
                        .fixedWidth = true,
                        .layoutDirection = L_right,
                        .notSelectable = true,
                        .wantGrowHorizontal = true,
                        .cornerRadius = 10
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = {.2, .3, .3},
                        .text = "Reset",
                        .padding = {10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = {resetBoard}
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = {.6, .3, .3},
                        .text = "Switch sides",
                        .padding = {10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = {switchSides},
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = {.6, .3, .3},
                        .text = "Flip Board",
                        .padding = {10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = {flipBoard},
                    }
                ),
                createElement(
                    (ElementSettings){
                        .color = {.6, .3, .3},
                        .text = "Close",
                        .padding = {10, 10, 10 ,10},
                        .onHover = defaultHoverFun,
                        .onClick = runTaskFun,
                        .task = {closeGame},
                    }
                )
            )
        )
    );
    Element* chessBoard = getElement("game board");;
    chessBoard->flags.isActive = false;
}

static void createStartScreen(Element* root) {
    addChildElements(root,
        addChildElements(
            createElement(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .posMode = POS_ABSOLUTE,
                    .childGap = 10,
                    .padding = {10,10,10,10},
                    .pos = {200, 200},
                    .name = "start screen",
                    .cornerRadius = 10
                }
            ),
            createElement(
                (ElementSettings){
                    .color = {.2, .3, .3},
                    .text = "Welcome to my Chess Game"
                }
            ),
            createElement(
                (ElementSettings){
                    .color = {.3, .3, .3},
                    .padding = {10,10,10,10},
                    .text = "Start",
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = startChessGameTask,
                    .cornerRadius = 10
                }
            ),
            createElement(
                (ElementSettings){
                    .color = {.8, .0, .0},
                    .padding = {10,10,10,10},
                    .text = "End it all",
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = closeProgram,
                    .cornerRadius = 10
                }
            )
        )
    );
    gui_setActive("start screen", false);
}

static void createEndScreen(Element* root) {
    addChildElements(root,
        addChildElements(
            createElement(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .posMode = POS_ABSOLUTE,
                    .childGap = 10,
                    .padding = {10,10,10,10},
                    .pos = {800, 200},
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
                    .padding = {10,10,10,10},
                    .text = "End it all",
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .task = closeProgram
                }
            )
        )
    );
    Element* endScreen = getElement("end screen");
    endScreen->flags.isActive = false;
}