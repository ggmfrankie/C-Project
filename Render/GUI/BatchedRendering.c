//
// Created by ertls on 11.02.2026.
//

#include "BatchedRendering.h"

#include "glad/gl.h"
#include "../Render.h"

#define MAX_GUI_VERTICES 16384
static GraphicsData graphicsData;

void initBatchedRendering() {

    glGenVertexArrays(1, &graphicsData.VAO);
    glGenBuffers(1, &graphicsData.VBO);

    glBindVertexArray(graphicsData.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);

    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, pos));

    glEnableVertexAttribArray(1); //pos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, uv));

    glEnableVertexAttribArray(2); //pos
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, color));

    glEnableVertexAttribArray(3); //pos
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, brightness));

    glBufferData(GL_ARRAY_BUFFER,
             MAX_GUI_VERTICES * sizeof(GuiVertex),
             NULL,
             GL_DYNAMIC_DRAW);
}

void renderBatchedQuads(GuiVertex *vertices, size_t length) {

}