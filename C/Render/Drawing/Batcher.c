#include "glad/gl.h"
#include "Batcher.h"
#include "../GUI/GuiElement.h"
#include "RenderTypes.h"
#include "DataStructures/CArrayList.h"
//
// Created by ertls on 04.03.2026.
//

static struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint SSBO;
} graphicsData;

#define MAX_GUI_INSTANCES 81920


void Batcher_init() {

    glGenVertexArrays(1, &graphicsData.VAO);
    glBindVertexArray(graphicsData.VAO);

    glGenBuffers(1, &graphicsData.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 MAX_GUI_VERTICES * sizeof(GuiVertex),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glGenBuffers(1, &graphicsData.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 MAX_GUI_INDICES * sizeof(uint32_t),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // pos (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, pos));
    // uv (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, uv));
    // bufferBinding (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT,
                           sizeof(GuiVertex), (void*)offsetof(GuiVertex, bufferBinding));
    // ID (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT,
                           sizeof(GuiVertex), (void*)offsetof(GuiVertex, ID));

    glBindVertexArray(0);


    glGenBuffers(1, &graphicsData.SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ElementInstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, graphicsData.SSBO);

}

void uploadBatchedQuads(GuiVertex **aVertices, int **aIndices) {
    glBindVertexArray(graphicsData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GuiVertex) * arrLen(*aVertices), *aVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * arrLen(*aIndices), *aIndices);
}

static ElementInstanceData instanceFromElement(const Element* e) {
    ElementInstanceData out = {};
    const float brightness = (e->state >= UI_STATE_HOVER && e->flags.canBeHovered) ? e->visuals.brightness - 0.2 : e->visuals.brightness;
    out.worldPos = e->dims.worldPos;
    out.color = (Vec4f){
        e->visuals.color.x * brightness,
        e->visuals.color.y * brightness,
        e->visuals.color.z * brightness,
        1.0f - e->visuals.transparency
    };
    out.atlasID = 0;

    return out;
}

static size_t ssboOffsetBytes(const int index) {
    return (size_t)index * sizeof(ElementInstanceData);
}

void uploadElementData(const Element* element) {
    if (!element) return;
    const int id = element->handle.ID;
    if (id < 0 || id >= MAX_GUI_INSTANCES) return;

    const ElementInstanceData data = instanceFromElement(element);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.SSBO);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        (GLintptr)ssboOffsetBytes(id),
        (GLsizeiptr)sizeof(ElementInstanceData),
        &data
    );
}