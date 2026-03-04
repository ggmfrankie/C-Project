#include "glad/gl.h"
#include "Batcher.h"
#include "../GUI/GuiElement.h"
//
// Created by ertls on 04.03.2026.
//

typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint SSBO;
} GraphicsData;

struct asd {
    InstanceData instanceData;
};

#define MAX_GUI_INSTANCES 8192

static GraphicsData graphicsData;

void initBatchedRendering() {

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
    // ID (location = 2) - integer attribute!
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT,
                           sizeof(GuiVertex), (void*)offsetof(GuiVertex, ID));
    // texID (location = 3) - integer attribute!
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT,
                           sizeof(GuiVertex), (void*)offsetof(GuiVertex, texID));

    glBindVertexArray(0);


    glGenBuffers(1, &graphicsData.SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, graphicsData.SSBO);

}

void uploadBatchedQuads(const GuiVertex *vertices, const int vt, const int* indices, const int id) {
    glBindVertexArray(graphicsData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GuiVertex) * vt, vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * id, indices);
}

static InstanceData instanceFromElement(const Element* e) {
    InstanceData out;
    out.brightness = e->visuals.brightness;
    out.pos        = toVec2f(e->dims.pos);
    out.color      = e->visuals.color;
    out.texture    = e->visuals.texture;
    return out;
}

static size_t ssboOffsetBytes(int index) {
    return (size_t)index * sizeof(InstanceData);
}

void uploadElementData(Element* element) {
    if (!element) return;
    int id = element->ID;
    if (id < 0 || id >= MAX_GUI_INSTANCES) return;

    InstanceData data = instanceFromElement(element);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.SSBO);


    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        (GLintptr)ssboOffsetBytes(id),
        (GLsizeiptr)sizeof(InstanceData),
        (const void*)&data
    );
}