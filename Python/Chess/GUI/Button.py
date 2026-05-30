import pygame
from pygame.color import Color

from Python.Utils.Vector import Vec2


class Button:
    def __init__(self, pos: Vec2, dims: Vec2, color: Color, hover_color: Color, action):
        self.rect = pygame.Rect(pos.x, pos.y, dims.x, dims.y)
        self.color = color
        self.hover_color = hover_color
        self.action = action
        self.is_hovered = False

    def draw(self, surface):
        color = self.hover_color if self.is_hovered else self.color
        pygame.draw.rect(surface, color, self.rect)

    def update(self, events):
        mouse_pos = pygame.mouse.get_pos()
        self.is_hovered = self.rect.collidepoint(mouse_pos)

        for event in events:
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                if self.rect.collidepoint(event.pos):
                    self.action()
