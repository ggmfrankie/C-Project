//
// Created by Stefan on 10.10.2025.
//
#include "../GUI/Mesh.h"

#include <string.h>

#include "glad/gl.h"
Mesh quadMesh;
void deleteMeshData(MeshData *meshData);

Mesh newMesh(const MeshData *meshData) {
    unsigned int VAO;
    unsigned int* VBOs = malloc(3 * sizeof(unsigned int));
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(3, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData->vertexCount * 3, meshData->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData->vertexCount * 3, meshData->normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData->vertexCount * 2, meshData->texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData->indexCount, meshData->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    deleteMeshData((MeshData*)meshData);

    return (Mesh){
        .indexCount = (int) meshData->indexCount,
        .vaoId = VAO,
        .vboId = VBOs,
        .eboId = EBO,
        .vboCount = 3,
        .render = Mesh_render
    };
}

void Mesh_render(const Mesh *mesh) {
    glBindVertexArray(mesh->vaoId);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

Mesh Mesh_loadSimpleQuad() {
    MeshData *meshData = malloc(sizeof(MeshData));
    if (!meshData) return (Mesh){0};

    meshData->vertexCount = 4;

    meshData->vertices = malloc(sizeof(float) * meshData->vertexCount * 3);

    const float tmpVertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
    memcpy(meshData->vertices, tmpVertices, sizeof(tmpVertices));

    meshData->normals = malloc(sizeof(float) * meshData->vertexCount * 3);
    const float tmpNormals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };
    memcpy(meshData->normals, tmpNormals, sizeof(tmpNormals));

    meshData->texCoords = malloc(sizeof(float) * meshData->vertexCount * 2);
    const float tmpTex[] = {
        0.0f, 0.0f,  // top-left
        0.0f, 1.0f,  // bottom-left
        1.0f, 0.0f,  // top-right
        1.0f, 1.0f   // bottom-right
    };
    memcpy(meshData->texCoords, tmpTex, sizeof(tmpTex));

    meshData->indexCount = 6;
    meshData->indices = malloc(sizeof(unsigned int) * meshData->indexCount);
    const unsigned int tmpIndices[] = {0, 1, 2, 2, 1, 3};
    memcpy(meshData->indices, tmpIndices, sizeof(tmpIndices));

    return newMesh(meshData);
}

void deleteMeshData(MeshData *meshData) {
    if (!meshData) return;
    if (meshData->vertices) free(meshData->vertices);
    if (meshData->normals) free(meshData->normals);
    if (meshData->texCoords) free(meshData->texCoords);
    if (meshData->indices) free(meshData->indices);
    free(meshData);
}