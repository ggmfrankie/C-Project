//
// Created by Stefan on 10.10.2025.
//

#ifndef C_MESH_H
#define C_MESH_H
#include "../Utils/ArrayList.h"

typedef struct MeshData {
    float* vertices;
    float* normals;
    float* texCoords;
    size_t vertexCount;
    int* indices;
    size_t indexCount;
} MeshData;

typedef struct Mesh {
    unsigned int vaoId;
    unsigned int* vboId;
    int vboCount;
    unsigned int eboId;
    int indexCount;
    void (*render)(const struct Mesh *mesh);
} Mesh;

ARRAY_LIST(Mesh, Mesh)

Mesh Mesh_loadSimpleQuad();
Mesh newMesh(const MeshData *meshData);
void Mesh_render(const Mesh *mesh);

#endif //C_MESH_H