import pygame
import sys

from pygame import Surface
from pygame.color import Color

from Python.Chess.GUI.Button import Button
from Python.Chess.Pieces.BasePiece import BasePiece
from Python.Chess.Utils.Vector import Vec2

from Python.Chess.Pieces.Rook import Rook
from Python.Chess.Pieces.King import King
from Python.Chess.Pieces.Queen import Queen
from Python.Chess.Pieces.Bishop import Bishop
from Python.Chess.Pieces.Knight import Knight
from Python.Chess.Pieces.Pawn import Pawn

class ChessBoard:

    def __init__(self):
        self.board: list[list[BasePiece | None]] = [[None] * 8 for _ in range(8)]
        self.buttons: list[Button] = []
        self.piece_sprites: dict[str, Surface] = {}
        self.screen = None


    def init(self):
        pygame.init()
        self.load_board()
        self.load_chess_pieces()                                            #Load sprites here

        screen_width = 600
        screen_height = 600
        self.screen = pygame.display.set_mode((screen_width, screen_height))

        dimensions = Vec2(75, 75)
        white = Color(255, 255, 255)
        black = Color(0, 0, 0)
        light_gray = Color(100, 100, 100)
        dark_gray = Color(50, 50, 50)

        for i in range(0, 8, 1):
            for j in range(0, 8, 1):
                isWhite = (i + j) % 2 == 0

                self.buttons.append(Button(
                    Vec2(i * dimensions.x, j * dimensions.y),
                    dimensions,
                    white if isWhite else black,
                    light_gray if isWhite else dark_gray,
                    None,
                    lambda r=i, c=j: self.on_square_clicked(r, c)
                ))

        pygame.display.set_caption("Chess")

    def load_board(self):
        self.board[0][0] = Rook(0)
        self.board[0][-1] = Rook(0)
        self.board[0][1] = Knight(0)
        self.board[0][-2] = Knight(0)
        self.board[0][2] = Bishop(0)
        self.board[0][-3] = Bishop(0)
        self.board[0][3] = Queen(0)
        self.board[0][-4] = King(0)

        self.board[-1][0] = Rook(1)
        self.board[-1][-1] = Rook(1)
        self.board[-1][1] = Knight(1)
        self.board[-1][-2] = Knight(1)
        self.board[-1][2] = Bishop(1)
        self.board[-1][-3] = Bishop(1)
        self.board[-1][3] = Queen(1)
        self.board[-1][-4] = King(1)

        for i in range(8):
            self.board[1][i] = Pawn(0)
            self.board[-2][i] = Pawn(1)


    def load_chess_pieces(self):
        self.load_sprite('b_bishop_1x_ns.png')
        self.load_sprite('b_king_1x_ns.png')
        self.load_sprite('b_knight_1x_ns.png')
        self.load_sprite('b_pawn_1x_ns.png')
        self.load_sprite('b_queen_1x_ns.png')
        self.load_sprite('b_rook_1x_ns.png')
        self.load_sprite('w_bishop_1x_ns.png')
        self.load_sprite('w_king_1x_ns.png')
        self.load_sprite('w_knight_1x_ns.png')
        self.load_sprite('w_pawn_1x_ns.png')
        self.load_sprite('w_queen_1x_ns.png')
        self.load_sprite('w_rook_1x_ns.png')


    def load_sprite(self, path: str):
        # BRO WIE SCHREIBT MAN RESOURCES Grrrr
        default_path = '../../Resources/Textures/ChessPieces/'      #Folder name falsch und resources falsch geschrieben
        sprite = pygame.image.load(default_path+path)
        parts = path.split('_',maxsplit=2)
        name = "_".join(parts[:2])                                  #.join() joined alle strings in den klammern mit "_" als Seperator [:2] bedeuted nimm in der liste nur die Einträge von 0 (wegelassen) bis ausschließlich index 2
        self.piece_sprites[name] = sprite


    def loop(self):
        running = True
        while running:
            events = pygame.event.get()
            for event in events:
                if event.type == pygame.QUIT:
                    running = False

            self.draw_gui(events)

            pygame.display.flip()

        pygame.quit()
        sys.exit()


    def draw_gui(self, events):
        for i, button in enumerate(self.buttons):
            row = i % 8                                #Whole number division
            col = i // 8
            piece = self.board[row][col]

            if piece is not None:
                button.set_sprite(self.piece_sprites[piece.get_identifier()])
            else:
                button.set_sprite(None)
            button.update(events)
            button.draw(self.screen)


    def on_square_clicked(self, row, col):
        print(f"Clicked at {row}/{col}")
