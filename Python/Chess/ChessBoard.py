import pygame 
import sys

class chess_board:
    test = 0

    board: list[list[int]]

    def __init__(self, count):
        self.test = count
        self.board = [[0] * 8 for _ in range(8)]
    

    def drawing():
        pygame.init()

        screen_width = 600
        screen_height = 600
        screen = pygame.display.set_mode((screen_width, screen_height))

        square = (75,75)
        white = (255,255,255)

        for i in range(0,8,1):
            for j in range(0,8,1):
                position = (i*square[0],j*square[1])
                if i % 2 == 0 and j % 2 == 0:
                    whitespace = pygame.Rect(position,square)
                    pygame.draw.rect(screen,white,whitespace)
                elif i % 2 != 0 and j % 2 != 0:  
                    whitespace = pygame.Rect(position,square)
                    pygame.draw.rect(screen,white,whitespace)

        pygame.display.set_caption("Chess")

        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False

            pygame.display.flip()

        pygame.quit()
        sys.exit() 

    

