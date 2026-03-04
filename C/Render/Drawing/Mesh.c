//
// Created by Stefan on 10.10.2025.
//
#include "Mesh.h"

#include <string.h>
#include <math.h>

#include <bemapiset.h>
#include "glad/gl.h"
#include "Render/GUI/GuiElement.h"
#include "Utils/Vector.h"

void deleteMeshData(MeshData *meshData);
inline float deg2rad(const float d) { return d * (M_PI / 180.0f); }


void getMeshFromElement(Element* element, GuiVertex *vertices, int *index) {
    int id = *index;
    for (int i = 0; i < 6; i++) {
        gm->vertices[i] = (GuiVertex){
            .hasTexture = element->flags.hasTexture,
            .brightness = element->brightness,
            .color = (Vec4f){element->color.x, element->color.y, element->color.z, 1.0f},
        };
    }


    vertices[id].pos = toVec2f(element->worldPos);
    vertices[id].uv = element->texture.uv0;
    id++;

    vertices[id].pos = (Vec2f){0.0f, 1.0f};
    vertices[id].uv = (Vec2f){element->texture.uv0.x, element->texture.uv1.y};
    id++;

    vertices[id].pos = (Vec2f){1.0f, 0.0f};
    vertices[id].uv = (Vec2f){element->texture.uv1.x, element->texture.uv0.y};
    id++;

    vertices[id].pos = (Vec2f){1.0f, 0.0f};
    vertices[id].uv = (Vec2f){element->texture.uv1.x, element->texture.uv0.y};
    id++;

    vertices[4].pos = (Vec2f){0.0f, 1.0f};
    vertices[4].uv = (Vec2f){element->texture.uv0.x, element->texture.uv1.y};

    vertices[5].pos = (Vec2f){1.0f, 1.0f};
    vertices[5].uv = (Vec2f){element->texture.uv1.x, element->texture.uv1.y};
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

Mesh Mesh_loadRoundedCornerMesh2(Element* element) {

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
        (Vec3f){width-radius, radius},
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
        (Vec3f){width-radius, height-radius},
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
        (Vec3f){radius, height-radius},
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