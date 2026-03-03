//
// Created by Stefan on 10.10.2025.
//
#include "../GUI/Mesh.h"

#include <string.h>
#include <math.h>

#include <bemapiset.h>
#include "glad/gl.h"
#include "Utils/Vector.h"

void deleteMeshData(MeshData *meshData);
inline float deg2rad(const float d) { return d * (M_PI / 180.0f); }

Mesh newMesh(MeshData meshData) {
    unsigned int VAO;
    unsigned int VBOs[3];
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(3, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.vertexCount * 3, meshData.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.vertexCount * 3, meshData.normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData.vertexCount * 2, meshData.texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.indexCount, meshData.indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    deleteMeshData(&meshData);

    return (Mesh){
        .indexCount = (int) meshData.indexCount,
        .vaoId = VAO,
        .vboId[0] = VBOs[0],
        .vboId[1] = VBOs[1],
        .vboId[2] = VBOs[2],
        .eboId = EBO,
        .vboCount = 3,
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


constexpr int numTriangle = 5;
static void Mesh_loadTriangleFan(Vec3f *verts, int* index, float rad, const Vec3f center) {
    constexpr float radStep = ((float)M_PI/2.0f)/numTriangle;

    for (int i = 0 ; i < numTriangle; i++) {
        rad += radStep * i;
        Vec3f *t = &verts[(*index)++];
        t->x = center.x + cosf(i * radStep);
        t->y = center.y + sinf(i * radStep);
    }
}

Mesh Mesh_loadRoundedCornerMesh(const float r) {
    constexpr float half = 0.5f*(float)M_PI;

    const float radius = max(r, 0.5);

    constexpr int maxVert = 64;
    Vec3f vertices[maxVert] = {};
    int vert = 0;
    int indices[maxVert] = {};
    int index = 0;

    vertices[vert++] = (Vec3f){0, radius};                 // leftUp
    vertices[vert++] = (Vec3f){radius, radius};            // topLeftCorner

    Mesh_loadTriangleFan(vertices, &vert, 3*half, vertices[vert-1]);
    for (int i = 1; i <= numTriangle; i++) {
        indices[index++] = 1;
        indices[index] = indices[-1 + index++];
        indices[index++] = i;
    }

    vertices[vert++] = (Vec3f){radius, 0};                 // topLeft

    indices[index++] = 1;
    indices[index++] = 2;
    indices[index++] = 3;


    vertices[vert++] = (Vec3f){1, radius};                 // rightUp
    vertices[vert++] = (Vec3f){1 - radius, 0};             // topRight
    vertices[vert++] = (Vec3f){1 - radius, radius};        // topRightCorner
    Mesh_loadTriangleFan(vertices, &vert, 0, vertices[vert-1]);

    vertices[vert++] = (Vec3f){1, 1 - radius};             // rightDown
    vertices[vert++] = (Vec3f){1 - radius, 1};             // bottomRight
    vertices[vert++] = (Vec3f){1 - radius, 1 - radius};    // bottomRightCorner
    Mesh_loadTriangleFan(vertices, &vert, half, vertices[vert-1]);

    vertices[vert++] = (Vec3f){0, 1 - radius};             // leftDown
    vertices[vert++] = (Vec3f){radius, 1};                 // bottomLeft
    vertices[vert++] = (Vec3f){radius, 1 - radius};        // bottomLeftCorner
    Mesh_loadTriangleFan(vertices, &vert, 2*half, vertices[vert-1]);


}

void deleteMeshData(MeshData *meshData) {
    if (!meshData) return;
    if (meshData->vertices) free(meshData->vertices);
    if (meshData->normals) free(meshData->normals);
    if (meshData->texCoords) free(meshData->texCoords);
    if (meshData->indices) free(meshData->indices);
}