//
// Created by Stefan on 28.10.2025.
//

#include "GuiElement.h"

Element newElement(Mesh *mesh, const short meshCount, const float x, const float y) {
    return (Element){
        .Mesh = mesh,
        .meshCount = meshCount,
        .isVisible = true,
        .onClick = NULL,
        .onHover = NULL,
        .x = x,
        .y = y
    };
}

void setUniforms(Element* element) {

}

