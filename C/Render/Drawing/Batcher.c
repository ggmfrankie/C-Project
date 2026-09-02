#include "glad/gl.h"
#include "Batcher.h"
#include "../GUI/GuiElement.h"
#include "RenderTypes.h"
#include "DataStructures/CArrayList.h"
#include "Makros/Makros.h"
//
// Created by ertls on 04.03.2026.
//

static struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint elementSSBO;
    GLuint meshSSBO;
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


    glGenBuffers(1, &graphicsData.elementSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.elementSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ElementInstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, graphicsData.elementSSBO);

    glGenBuffers(1, &graphicsData.meshSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.meshSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(MeshInstanceData) * MAX_GUI_INSTANCES, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, graphicsData.meshSSBO);
}

static void uploadVertices(const GuiVertex *aVertices, const int *aIndices) {
#if GUI_DEBUG && GUI_DEBUG_TRACK_VERTICES
    only_every_do(100,
        printf("Number of vertices: %llu\n", arrLen(aVertices))
    );
#endif

    glBindVertexArray(graphicsData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GuiVertex) * arrLen(aVertices), aVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * arrLen(aIndices), aIndices);
}

static void uploadElementData(const ElementInstanceData* aElementData) {
    assert(aElementData != nullptr);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.elementSSBO);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        0,
        arrLen(aElementData) * sizeof(ElementInstanceData),
        aElementData
    );
}

static void uploadMeshData(const MeshInstanceData* aMeshData) {
    assert(aMeshData != nullptr);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphicsData.meshSSBO);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
        0,
        arrLen(aMeshData) * sizeof(MeshInstanceData),
        aMeshData
    );
}

void uploadMeshes(const MeshAccumulator* accumulator) {
    uploadVertices(accumulator->aVertices, accumulator->aIndices);
    uploadElementData(accumulator->aElementData);
    uploadMeshData(accumulator->aMeshData);
}