//
// Created by ertls on 27.07.2026.
//

#pragma once
#include <array>
#include <string>


class TicTacToe {
public:
    TicTacToe();

    bool set(int row, int col, char c);

    bool makeMove(int row, int col);

    [[nodiscard]] std::string getBoard() const;

private:
    enum class Turn {
        X,
        O
    };
    std::array<char, 10> mBoard;
    Turn mTurn;
};
