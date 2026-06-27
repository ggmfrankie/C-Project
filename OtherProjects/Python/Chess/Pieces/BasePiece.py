from typing import Self

from OtherProjects.Python.Chess.Pieces.Move import Move
from OtherProjects.Python.Chess.Utils.Vector import Vec2


class BasePiece:

    def __init__ (self, color:int):
        self.color = color
        self.color_string = 'w' if color == 1 else 'b'

    def get_diagonal_moves(self, pos: Vec2, board: list[list[Self | None]], direction, limit = -1) -> list[Move]:
        moves: list[Move] = []


        for i in range(8):
            match direction:
                case 0:
                    row = pos.x - i
                    col = pos.y + i
                    pass
                case 1:
                    row = pos.x + i
                    col = pos.y + i
                    pass
                case 2:
                    row = pos.x + i
                    col = pos.y - i
                    pass
                case 3:
                    row = pos.x - i
                    col = pos.y - i
                    pass
                case _ :
                    return []

            if row > 7 or col > 7 or row < 0 or col < 0:
                break

            figure = board[row][col]
            if figure is None:
                moves.append(Move(row, col, False, 0, False, False))
            elif figure.color == self.color:
                break
            else:
                moves.append(Move(row, col, True, 10, False, False))
                break

        return []

    def get_orthogonal_moves(self, direction, limit = -1) -> list[Move]:
        #TODO
        return []

    def get_color(self):
        return self.color_string

    def get_identifier(self) -> str:
        pass

    def get_moves(self, pos: Vec2, board: list[list[Self | None]]) -> list[Move]:
        return []