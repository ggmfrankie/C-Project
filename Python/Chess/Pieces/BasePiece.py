from Python.Chess.Pieces.Move import Move

class BasePiece:

    def __init__ (self, color:int):
        self.color = color
        self.color_string = 'w' if color == 1 else 'b'

    def get_diagonal_moves(self, direction, limit = -1) -> list[Move]:
        #TODO
        return []

    def get_orthogonal_moves(self, direction, limit = -1) -> list[Move]:
        #TODO
        return []

    def get_color(self):
        return self.color_string

    def get_identifier(self) -> str:
        pass