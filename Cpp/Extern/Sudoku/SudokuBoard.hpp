//
// Created by ertls on 04.05.2026.
//

#pragma once
#include "Utils/Math/ggmdef.hpp"
#include <array>

namespace Games {
    class SudokuBoard {
        std::array<std::array<ggm::u8, 9>, 9> mBoard{};

    public:
        SudokuBoard() = default;

        void print() const;
        void createBoard();
        void solveBoard();
        void addNumbersRand(int count);
    };

    void Sudoku();
} // Games