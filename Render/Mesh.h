//
// Created by Stefan on 10.10.2025.
//

#ifndef C_MESH_H
#define C_MESH_H
#include "../Utils/ArrayList.h"


typedef struct Mesh {
    unsigned int vaoId;
    unsigned int* vboId;
    int vboCount;
    unsigned int eboId;
    int indexCount;
} Mesh;

typedef struct MeshData {
    float* vertices;
    float* normals;
    float* texCoords;
    int vertexCount;
    int* indices;
    int indexCount;
} MeshData;

ARRAY_LIST(Mesh, Mesh)

Mesh loadSimpleMesh();
Mesh initMesh(const MeshData *meshData);
void renderMesh(const Mesh *mesh);

#endif //C_MESH_H