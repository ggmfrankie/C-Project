import pygame
import sys

from pygame import Surface
from pygame.color import Color
from pygame.surface import SurfaceType

from Python.Chess.GUI.Button import Button
from Python.Chess.Pieces.base_piece import base_piece
from Python.Utils.Vector import Vec2



class chess_board:

    def __init__(self, count):
        self.test = count
        self.board: list[list[base_piece | None]] = [[None] * 8 for _ in range(8)]
        self.buttons: list[Button] = []
        self.piece_sprites: dict[str, Surface] = {}
        self.screen = None


    def init(self):
        pygame.init()

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


    def load_sprite(self, path):
        default_path = '../Ressources/Textures/Chess_pieces/'
        sprite = pygame.image.load(default_path+path)
        path_piece_name, idk = path.split('_',2)
        self.piece_sprites[path_piece_name] = sprite

    def loop(self):

        running = True
        while running:
            events = pygame.event.get()
            for event in events:
                if event.type == pygame.QUIT:
                    running = False

            for button in self.buttons:
                button.update(events)
                button.draw(self.screen)

            pygame.display.flip()

        pygame.quit()
        sys.exit()

    def update(self):
        for i, button in enumerate(self.buttons):
            row = i / 8
            col = i % 8
            piece = self. board



    def on_square_clicked(self, row, col):
        print(f"Clicked at {row}/{col}")
