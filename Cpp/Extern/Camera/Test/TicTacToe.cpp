//
// Created by ertls on 27.07.2026.
//

#include "TicTacToe.hpp"

#include <string>
#include <sstream>

using std::string;

TicTacToe TicTacToe::GAME;

TicTacToe::TicTacToe() :
    mBoard(),
    mTurn(Turn::X)
{
    mBoard.fill(' ');
}

bool TicTacToe::set(const int row, const int col, const char c) {
    if (row < 0 || col < 0 || row > 2 || col > 2) return false;
    if (mBoard[row*3 + col] != ' ') return false;
    mBoard[row*3 + col] = c;
    return true;
}

bool TicTacToe::makeMove(const int row, const int col) {
    return set(row, col,
        (mTurn == Turn::X) ? 'X' : 'O'
    );
}

string TicTacToe::getBoard() const {
    std::ostringstream output;
    for (int i = 0; i < mBoard.size(); ++i) {
        output << mBoard[i];
        if ((i+1)%3 == 0) output << '\n';
    }
    return output.str();
}

TicTacToe& TicTacToe::New() {
    return GAME = TicTacToe();
}

TicTacToe& TicTacToe::Get() {
    return GAME;
}
