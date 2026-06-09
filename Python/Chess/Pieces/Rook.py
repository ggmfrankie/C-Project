from BasePiece import BasePiece


class Rook(BasePiece):
    def __init__(self, color: int):
        super().__init__(color)

    def getMoves(self):
        pass

    def get_identifier(self):
        return '_'.join([super().get_color(),'Rook'])