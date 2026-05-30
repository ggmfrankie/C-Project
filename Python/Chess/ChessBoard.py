class ChessBoard:

    def __init__(self, count):
        self.test = count
        self.board: list[list[int]] = [[0] * 8 for _ in range(8)]

    def draw(self):
        pass
