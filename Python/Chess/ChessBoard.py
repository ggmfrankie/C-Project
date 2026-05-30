class chess_board:
    test = 0

    board: list[list[int]]

    def __init__(self, count):
        self.test = count
        self.board = [[0] * 8 for _ in range(8)]

    

