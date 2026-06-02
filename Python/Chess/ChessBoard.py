import pygame
import sys

from pygame import Surface
from pygame.color import Color

from Python.Chess.GUI.Button import Button
from Python.Chess.Pieces.BasePiece import BasePiece
from Python.Utils.Vector import Vec2

class ChessBoard:

    def __init__(self, count):
        self.test = count
        self.board: list[list[BasePiece | None]] = [[None] * 8 for _ in range(8)]
        self.buttons: list[Button] = []
        self.piece_sprites: dict[str, Surface] = {}
        self.screen = None


    def init(self):
        pygame.init()
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
            row = i // 8
            col = i % 8
            piece = self.board[row][col]

            button.set_sprite(self.piece_sprites["b_king"])
            button.update(events)
            button.draw(self.screen)



    def on_square_clicked(self, row, col):
        print(f"Clicked at {row}/{col}")
