//
// Created by ertls on 04.05.2026.
//

#pragma once
#include "../../../Cpp/Utils/ggmdef.hpp"
#include <array>

namespace Games {
    class SudokuBoard {
        std::array<std::array<ggm::u8, 9>, 9> mBoard{};

    public:
        SudokuBoard() = default;

        void print() const;
        void createBoard();

        int check3by3(int u, int w, ggm::u8 num) const;

        int canPlace(int row, int col, ggm::u8 num) const;

        void solveBoard();
        void addNumbersRand(int count);

        void initBoard();
    };

    void Sudoku();
} // Games