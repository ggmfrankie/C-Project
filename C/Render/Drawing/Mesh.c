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

inline float deg2rad(const float d) { return d * (M_PI / 180.0f); }

struct ArcInfo {
    int start;
    int end;
    int corner;
};

static struct ArcInfo Mesh_triangulate(const Vec2f corner, const float radius, GuiVertex *verts, int* vt, int* indices, int* id, float startAngle, float endAngle, int numTriangles) {
    float radStep = (endAngle - startAngle)/(float)numTriangles;
    int cornerIndex = *vt;

    verts[(*vt)++].pos = corner;

    int startIndex = *vt;

    verts[(*vt)++].pos = (Vec2f){
        corner.x + cosf(startAngle)*radius,
        corner.y + sinf(startAngle)*radius,
    };

    for (int i = 1 ; i <= numTriangles; i++) {


        verts[*vt].pos = (Vec2f){
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

void Mesh_loadRoundedCornerMesh2(const Element* element, GuiVertex *vertices, int *vt, int* indices, int* id) {
    const float width = (float)element->dims.actualWidth;
    const float height = (float)element->dims.actualHeight;
    const float radius = min((float)element->dims.cornerRadius, min(width, height) * 0.5f);

    constexpr float r90 = (float)M_PI * 0.5f;
    constexpr int numTriangles = 12;

    int start = *vt;

    auto tl = Mesh_triangulate(
        (Vec2f){radius, radius},
        radius,
        vertices,
        vt,
        indices,
        id,
        2*r90,
        3*r90,
        numTriangles
    );

    auto tr = Mesh_triangulate(
        (Vec2f){width-radius, radius},
        radius,
        vertices,
        vt,
        indices,
        id,
        3*r90,
        4*r90,
        numTriangles
    );

    auto br = Mesh_triangulate(
        (Vec2f){width-radius, height-radius},
        radius,
        vertices,
        vt,
        indices,
        id,
        0,
        r90,
        numTriangles
    );

    auto bl = Mesh_triangulate(
        (Vec2f){radius, height-radius},
        radius,
        vertices,
        vt,
        indices,
        id,
        r90,
        2*r90,
        numTriangles
    );
    Mesh_connectFans(&tl, &tr, indices, id);
    Mesh_connectFans(&tr, &br, indices, id);
    Mesh_connectFans(&br, &bl, indices, id);
    Mesh_connectFans(&bl, &tl, indices, id);

    indices[(*id)++] = tl.corner;
    indices[(*id)++] = bl.corner;
    indices[(*id)++] = tr.corner;

    indices[(*id)++] = tr.corner;
    indices[(*id)++] = bl.corner;
    indices[(*id)++] = br.corner;

    const Vec2f uv0 = element->visuals.texture.uv0;
    const Vec2f uv1 = element->visuals.texture.uv1;

    for (int i = start; i < *vt; i++) {
        GuiVertex* p = &vertices[i];

        const float uNorm = p->pos.x / width;
        const float vNorm = p->pos.y / height;

        p->uv = (Vec2f){
            uv0.x * (1.0f - uNorm) + uv1.x * uNorm,
            uv0.y * (1.0f - vNorm) + uv1.y * vNorm
        };
        p->ID = element->ID;
        p->texID = 0;
    }
}