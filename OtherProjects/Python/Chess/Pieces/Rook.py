from typing import Self

from OtherProjects.Python.Chess.Pieces.BasePiece import BasePiece
from OtherProjects.Python.Chess.Pieces.Move import Move
from OtherProjects.Python.Chess.Utils.Vector import Vec2


class Rook(BasePiece):
    def __init__(self, color: int):
        super().__init__(color)

    def get_moves(self, pos: Vec2, board: list[list[Self | None]]) -> list[Move]:
        return []

    def get_identifier(self):
        return '_'.join([super().get_color(),'rook'])