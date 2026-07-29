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

    int SudokuBoard::check3by3(int u, int w, ggm::u8 num) const {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (mBoard[3*u+i][3*w+j] == num) return 0;
            }
        }
        return 1;
    }

    int SudokuBoard::canPlace(int row, int col, ggm::u8 num) const {
        for (const auto c: mBoard[row]) {
            if (c == num) return 0;
        }

        for (auto& r: mBoard) {
            if (r[col] == num) return 0;
        }

        int u = row/3;
        int w = col/3;

        return check3by3(u, w, num);
    }

    void SudokuBoard::solveBoard() {
        for (int num = 1; num < 10; ++num) {
            for (int i = 0; i < mBoard.size(); ++i) {
                int count = 0;
                int j = 0;
                for (; j < mBoard.size(); ++j) {

                    if (mBoard[i][j] == 0) count += canPlace(i, j, num);

                }
                if (count == 1) mBoard[i][j] = num;
                count = 0;

                for (j = 0; j < mBoard.size(); ++j) {
                    if (mBoard[j][i] == 0) count += canPlace(j, i, num);

                }
                if (count == 1) mBoard[j][i] = num;
                count = 0;

                for (int a = 0; a < 3; ++a) {
                    int r = 0;
                    int c = 0;
                    for (int u = 0; u < 3; ++u) {
                        for (int w = 0; w < 3; ++w) {
                            r = u+3*i;
                            c = w+3*a;
                            if (mBoard[r][c] == 0) count += canPlace(r, c, num);

                        }
                    }
                    if (count == 1) mBoard[r][c] = num;
                }
            }
        }
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

    void SudokuBoard::initBoard() {
        mBoard = {
            std::array<ggm::u8, 9>{5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        };
    }

    void Sudoku() {
        SudokuBoard board{};
        board.initBoard();
        board.solveBoard();
        board.print();
    }
} // Games