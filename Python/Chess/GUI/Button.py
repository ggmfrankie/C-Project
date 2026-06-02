import pygame
from pygame import Surface
from pygame.color import Color

from Python.Utils.Vector import Vec2



class Button:
    def __init__(self, pos: Vec2, dims: Vec2, color: Color, hover_color: Color, sprite: Surface | None = None, action = None):
        self.rect = pygame.Rect(pos.x, pos.y, dims.x, dims.y)
        self.color = color
        self.hover_color = hover_color
        self.action = action
        self.is_hovered = False
        self.sprite = sprite

    def draw(self, surface: Surface):
        color = self.hover_color if self.is_hovered else self.color
        pygame.draw.rect(surface, color, self.rect)

       # surface.blit(self.sprite, )


    def update(self, events):
        mouse_pos = pygame.mouse.get_pos()
        self.is_hovered = self.rect.collidepoint(mouse_pos)

        for event in events:
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                if self.rect.collidepoint(event.pos):
                    self.action()
