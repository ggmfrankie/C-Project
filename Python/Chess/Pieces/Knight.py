from typing import Self

from Python.Chess.Pieces.BasePiece import BasePiece
from Python.Chess.Pieces.Move import Move
from Python.Chess.Utils.Vector import Vec2


class Knight(BasePiece):
    def __init__(self, color: int):
        super().__init__(color)

    def check(self, x:int,y:int,board: list[list[Self | None]], moves:list[Move]):
        if y > 7 or x > 7 or y < 0 or x < 0:
            return

        figure = board[y][x]
        if figure is None:
            moves.append(Move(y, x, False, 0, False, False))
        elif figure.color != self.color:
            moves.append(Move(y, x, True, 10, False, False))
        return

    def get_moves(self, pos: Vec2, board: list[list[Self | None]]) -> list[Move]:
        moves: list[Move] = []

        self.check(pos.x + 1, pos.y - 2, board, moves)
        self.check(pos.x - 1, pos.y - 2, board, moves)
        self.check(pos.x + 1, pos.y + 2, board, moves)
        self.check(pos.x - 1, pos.y + 2, board, moves)
        self.check(pos.x + 2, pos.y - 1, board, moves)
        self.check(pos.x - 2, pos.y - 1, board, moves)
        self.check(pos.x + 2, pos.y + 1, board, moves)
        self.check(pos.x - 2, pos.y + 1, board, moves)





        return moves

    def get_identifier(self):
        return '_'.join([super().get_color(),'knight'])