//
// Created by Stefan on 10.10.2025.
//
#include "../GUI/Mesh.h"

#include <math.h>
#include <string.h>

#include "glad/gl.h"
#include "Utils/Vector.h"

#define min(i, j) (((i) < (j)) ? (i) : (j))

void deleteMeshData(MeshData *meshData);

Mesh newMesh(const MeshData meshData) {
    unsigned int VAO;
    unsigned int* VBOs = malloc(3 * sizeof(unsigned int));
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(3, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.vertexCount * 3, meshData.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.vertexCount * 3, meshData.normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.vertexCount * 2, meshData.texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.indexCount, meshData.indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return (Mesh){
        .indexCount = (int) meshData.indexCount,
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
    MeshData meshData;

    meshData.vertexCount = 4;

    meshData.vertices = malloc(sizeof(float) * meshData.vertexCount * 3);

    const float tmpVertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
    memcpy(meshData.vertices, tmpVertices, sizeof(tmpVertices));

    meshData.normals = malloc(sizeof(float) * meshData.vertexCount * 3);
    const float tmpNormals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };
    memcpy(meshData.normals, tmpNormals, sizeof(tmpNormals));

    meshData.texCoords = malloc(sizeof(float) * meshData.vertexCount * 2);
    const float tmpTex[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };
    memcpy(meshData.texCoords, tmpTex, sizeof(tmpTex));

    meshData.indexCount = 6;
    meshData.indices = malloc(sizeof(unsigned int) * meshData.indexCount);
    const unsigned int tmpIndices[] = {0, 1, 2, 2, 1, 3};
    memcpy(meshData.indices, tmpIndices, sizeof(tmpIndices));

    return newMesh(meshData);
}

Mesh Mesh_loadNinePatchMesh() {
    MeshData meshData = {};

    int patchIndices[] = {
                0, 1, -1, 0,
                2, 3, 4, 2,
                -2, 5, 6, -2,
                0, 1, -1, 0
    };
}

struct ArcInfo {
    int start;
    int end;
    int corner;
};


static struct ArcInfo Mesh_triangulate(Vec3f corner, float radius, Vec3f *verts, int* vt, int* indices, int* id, float startAngle, float endAngle, int numTriangles) {
    float radStep = (endAngle - startAngle)/(float)numTriangles;
    int cornerIndex = *vt;

    verts[(*vt)++] = corner;
    int startIndex = *vt;

    verts[(*vt)++] = (Vec3f){
        corner.x + cosf(startAngle)*radius,
        corner.y + sinf(startAngle)*radius,
    };

    for (int i = 1 ; i <= numTriangles; i++) {


        verts[*vt] = (Vec3f){
            corner.x + cosf(startAngle + i * radStep)*radius,
            corner.y + sinf(startAngle + i * radStep)*radius,
        };

        const int prev = *vt - 1;
        const int curr = *vt;

        indices[(*id)++] = cornerIndex;
        indices[(*id)++] = prev;
        indices[(*id)++] = curr;

        (*vt)++;
    }
    return (struct ArcInfo){startIndex, *vt-1, cornerIndex};
}

void Mesh_connectFans(struct ArcInfo* a1, struct ArcInfo* a2, int* indices, int* id) {
    indices[(*id)++] = a1->end;
    indices[(*id)++] = a1->corner;
    indices[(*id)++] = a2->start;

    indices[(*id)++] = a2->corner;
    indices[(*id)++] = a2->start;
    indices[(*id)++] = a1->corner;
}

Mesh Mesh_loadRoundedCornerMesh2(const float r) {
    const float radius = min(r, 0.5f);
    constexpr float r90 = (float)M_PI * 0.5f;
    constexpr int numTriangles = 12;

    Vec3f vertices[256] = {};
    int vert = 0;

    int indices[512] = {};
    int index = 0;

    auto tl = Mesh_triangulate(
        (Vec3f){radius, radius},
        radius,
        vertices,
        &vert,
        indices,
        &index,
        2*r90,
        3*r90,
        numTriangles
    );

    auto tr = Mesh_triangulate(
        (Vec3f){1-radius, radius},
        radius,
        vertices,
        &vert,
        indices,
        &index,
        3*r90,
        4*r90,
        numTriangles
    );

    auto br = Mesh_triangulate(
        (Vec3f){1-radius, 1-radius},
        radius,
        vertices,
        &vert,
        indices,
        &index,
        0,
        r90,
        numTriangles
    );

    auto bl = Mesh_triangulate(
        (Vec3f){radius, 1-radius},
        radius,
        vertices,
        &vert,
        indices,
        &index,
        r90,
        2*r90,
        numTriangles
    );
    Mesh_connectFans(&tl, &tr, indices, &index);
    Mesh_connectFans(&tr, &br, indices, &index);
    Mesh_connectFans(&br, &bl, indices, &index);
    Mesh_connectFans(&bl, &tl, indices, &index);

    indices[index++] = tl.corner;
    indices[index++] = bl.corner;
    indices[index++] = tr.corner;

    indices[index++] = tr.corner;
    indices[index++] = bl.corner;
    indices[index++] = br.corner;

    Vec2f *glTex = malloc(sizeof(Vec2f) * vert);
    for (int i = 0; i < vert; i++) {
        const Vec3f* p = &vertices[i];
        glTex[i] = (Vec2f){p->x, p->y};
    }

    Vec3f *glVerts = malloc(sizeof(Vec3f) * vert);
    memcpy(glVerts, vertices, sizeof(Vec3f) * vert);

    int *glInd = malloc(sizeof(int) * index);
    memcpy(glInd, indices, sizeof(int) * index);

    Vec3f *glNorms = calloc(sizeof(Vec3f), vert);

    return newMesh((MeshData){(float*)glVerts, (float*)glNorms, (float*)glTex, vert, glInd, index});
}

void deleteMeshData(MeshData *meshData) {
    if (!meshData) return;
    if (meshData->vertices) free(meshData->vertices);
    if (meshData->normals) free(meshData->normals);
    if (meshData->texCoords) free(meshData->texCoords);
    if (meshData->indices) free(meshData->indices);
    free(meshData);
}