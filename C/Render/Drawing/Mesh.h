//
// Created by Stefan on 10.10.2025.
//

#ifndef C_MESH_H
#define C_MESH_H
#include "../../Utils/ArrayList.h"
#include "../../Utils/SimpleArray.h"
#include "Render/GUI/GuiElement.h"

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
    unsigned int vboId[3];
    int vboCount;
    unsigned int eboId;
    int indexCount;
    int patchIndices[16];
} Mesh;

ARRAY_LIST(Mesh, Mesh)

Mesh Mesh_loadSimpleQuad();
Mesh Mesh_loadRoundedCornerMesh2(Element* element);
Mesh newMesh(MeshData meshData);
void Mesh_render(const Mesh *mesh);

#endif //C_MESH_H