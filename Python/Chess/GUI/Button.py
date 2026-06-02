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

        #If button has a sprite, draw it
        if self.sprite is not None:
            sprite_rect = self.sprite.get_rect(center=self.rect.center)
            surface.blit(self.sprite, sprite_rect)


    def update(self, events):
        mouse_pos = pygame.mouse.get_pos()
        self.is_hovered = self.rect.collidepoint(mouse_pos)

        for event in events:
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                if self.rect.collidepoint(event.pos):
                    self.action()

    def set_sprite(self, sprite: Surface):
        #Rescales the sprite to be the same dimensions as the button itself
        self.sprite = pygame.transform.smoothscale(
            sprite,
            (self.rect.width, self.rect.height)
        )