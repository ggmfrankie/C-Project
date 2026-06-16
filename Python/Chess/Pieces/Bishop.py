from typing import Self

from Python.Chess.Pieces.BasePiece import BasePiece
from Python.Chess.Pieces.Move import Move
from Python.Chess.Utils.Vector import Vec2


class Bishop(BasePiece):
    def __init__(self, color: int):
        super().__init__(color)

    def get_moves(self, pos: Vec2, board: list[list[Self | None]]) -> list[Move]:
        return (super().get_diagonal_moves(pos, board, 0) +
                super().get_diagonal_moves(pos, board, 1) +
                super().get_diagonal_moves(pos, board, 2) +
                super().get_diagonal_moves(pos, board, 3)
                )

    def get_identifier(self):
        return '_'.join([super().get_color(),'bishop'])