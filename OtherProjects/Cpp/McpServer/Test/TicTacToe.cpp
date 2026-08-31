//
// Created by ertls on 27.07.2026.
//

#include "TicTacToe.hpp"

#include <string>
#include <sstream>
#include <iostream>

using std::string;

TicTacToe::TicTacToe() :
    mBoard(),
    mTurn(Turn::X)
{
    mBoard.fill('#');
}

bool TicTacToe::set(const int row, const int col, const char c) {
    if (row < 0 || col < 0 || row > 2 || col > 2) return false;
    if (mBoard[row*3 + col] != '#') return false;
    mBoard[row*3 + col] = c;
    return true;
}

bool TicTacToe::makeMove(const int row, const int col) {
    bool success = set(row, col,
        (mTurn == Turn::X) ? 'X' : 'O'
    );
    switchTurn();
    return success;
}

string TicTacToe::getBoard() const {
    std::ostringstream output;
    output << '|';
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            output << mBoard[i*3 + j];
        }
        output << '|';
    }
    return output.str();
}

void TicTacToe::printBoard() {
    std::cerr << Get().getBoard() << std::endl;
}

TicTacToe& TicTacToe::Get() {
    static TicTacToe Game{}; 
    return Game;
}

void TicTacToe::switchTurn() {
    mTurn = (mTurn == Turn::O) ? Turn::X : Turn::O;
}

bool TicTacToe::isWinning() {
    constexpr std::array<std::array<int, 3>, 8> WinningLines{{
        {{0, 1, 2}},
        {{3, 4, 5}},
        {{6, 7, 8}},
        {{0, 3, 6}},
        {{1, 4, 7}},
        {{2, 5, 8}},
        {{0, 4, 8}},
        {{2, 4, 6}}
    }}; 

    for (const auto& lines : WinningLines)
    {
        if (lines[0] == lines[1] && lines[1] == lines[2]) return true;
    }
    
    return false;
}
