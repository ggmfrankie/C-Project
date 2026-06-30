from typing import Self

from OtherProjects.Python.Chess.Pieces.Move import Move
from OtherProjects.Python.Chess.Utils.Vector import Vec2


class BasePiece:

    def __init__ (self, color:int):
        self.color = color
        self.color_string = 'w' if color == 1 else 'b'

    def get_diagonal_moves(self, pos: Vec2, board: list[list[Self | None]], direction) -> list[Move]:
        moves: list[Move] = []


        for i in range(1,8):
            match direction:
                case 0:
                    y = pos.y - i
                    x = pos.x + i
                    pass
                case 1:
                    y = pos.y + i
                    x = pos.x + i
                    pass
                case 2:
                    y = pos.y + i
                    x = pos.x - i
                    pass
                case 3:
                    y = pos.y - i
                    x = pos.x - i
                    pass
                case _ :
                    return []

            if y > 7 or x > 7 or y < 0 or x < 0:
                break

            figure = board[y][x]
            if figure is None:
                moves.append(Move(y, x, False, 0, False, False))
            elif figure.color == self.color:
                break
            else:
                moves.append(Move(y, x, True, 10, False, False))
                break

        return moves

    def get_orthogonal_moves(self, pos: Vec2, board: list[list[Self | None]], direction) -> list[Move]:
        moves: list[Move] = []

        for i in range(1, 8):
            match direction:
                case 0:
                    y = pos.y
                    x = pos.x + i
                    pass
                case 1:
                    y = pos.y + i
                    x = pos.x
                    pass
                case 2:
                    y = pos.y
                    x = pos.x - i
                    pass
                case 3:
                    y = pos.y - i
                    x = pos.x
                    pass
                case _:
                    return []

            if y > 7 or x > 7 or y < 0 or x < 0:
                break

            figure = board[y][x]
            if figure is None:
                moves.append(Move(y, x, False, 0, False, False))
            elif figure.color == self.color:
                break
            else:
                moves.append(Move(y, x, True, 10, False, False))
                break

        return moves

    def get_color(self):
        return self.color_string

    def get_identifier(self) -> str:
        pass

    def get_moves(self, pos: Vec2, board: list[list[Self | None]]) -> list[Move]:
        return []