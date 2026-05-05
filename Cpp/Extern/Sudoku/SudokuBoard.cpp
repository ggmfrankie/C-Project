//
// Created by ertls on 04.05.2026.
//

#include "SudokuBoard.hpp"
#include <iostream>

namespace Games {
    void SudokuBoard::print() const {
        puts("-----------------");
        for (const auto& row: mBoard) {
            for (const int p : row) {
                std::cout << p << " ";
            }
            puts("");
        }
        puts("-----------------");
    }

    void SudokuBoard::createBoard() {

    }

    void SudokuBoard::solveBoard() {

    }

    void SudokuBoard::addNumbersRand(int count) {
        int placed = 0;
        while (placed < count) {
            int x = rand() % 10;
            int y = rand() % 10;

            int num = rand() % 10;

            if (mBoard[x][y] == 0) {
                mBoard[x][y] = num;
                ++placed;
            }
        }
    }

    void Sudoku() {
        SudokuBoard board{};
        board.addNumbersRand(10);
        board.print();
    }
} // Games