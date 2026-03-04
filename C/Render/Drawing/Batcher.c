#include "glad/gl.h"
#include "Batcher.h"
#include "../GUI/GuiElement.h"
//
// Created by ertls on 04.03.2026.
//
typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint SSBO;
} GraphicsData;

typedef struct {
    float brightness;
    Vec2f pos;
    Vec4f color;
    Texture texture;
    char _padding[20];
} InstanceData;

struct asd {
    InstanceData instanceData;
};

#define MAX_GUI_INSTANCES 8192

static GraphicsData graphicsData;

void initBatchedRendering() {

    glGenVertexArrays(1, &graphicsData.VAO);
    glGenBuffers(1, &graphicsData.VBO);

    glBindVertexArray(graphicsData.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);

    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, pos));

    glEnableVertexAttribArray(1); //uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, uv));

    glBufferData(GL_ARRAY_BUFFER,
             MAX_GUI_VERTICES * sizeof(GuiVertex),
             nullptr,
             GL_DYNAMIC_DRAW);



    glGenBuffers(1, &graphicsData.SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(InstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, graphicsData.SSBO);

}

void renderBatchedQuads(const GLuint atlasId, const GuiVertex *vertices, const int length) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasId);
    glBindVertexArray(graphicsData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(GuiVertex) * length, vertices);

    glDrawArrays(GL_TRIANGLES, 0, length);
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
    int id = element->instanceIndex;
    if (id < 0 || id >= MAX_GUI_INSTANCES) return;

    InstanceData data = instanceFromElement(element);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.SSBO);


    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        (GLintptr)ssboOffsetBytes(id),
        (GLsizeiptr)sizeof(InstanceData),
        (const void*)&data
    );
}