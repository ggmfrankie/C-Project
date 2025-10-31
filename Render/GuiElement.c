//
// Created by Stefan on 28.10.2025.
//

#include "GuiElement.h"

Element newElement(Mesh *mesh, const short meshCount, const Vec2f pos, const int width, const int height, Texture* texture) {
    return (Element){
        .Mesh = mesh,
        .meshCount = meshCount,
        .isVisible = true,
        .onClick = NULL,
        .onHover = NULL,
        .pos = pos,
        .width = (float) width,
        .height = (float) height,
        .texture = texture
    };
}

