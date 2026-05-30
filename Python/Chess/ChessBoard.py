import pygame
import sys

from pygame import Surface
from pygame.color import Color
from pygame.surface import SurfaceType

from Python.Chess.GUI.Button import Button
from Python.Utils.Vector import Vec2


class chess_board:

    def __init__(self, count):
        self.test = count
        self.board: list[list[int]] = [[0] * 8 for _ in range(8)]
        self.buttons: list[Button] = []
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

    def on_square_clicked(self, row, col):
        print(f"Clicked at {row}/{col}")
