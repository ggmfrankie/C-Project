//
// Created by Stefan on 10.10.2025.
//
#include "Mesh.h"

#include <string.h>
#include <math.h>
#include "../../Utils/Makros/Makros.h"

#include "RenderTypes.h"
#include "Render/GUI/GuiElement.h"
#include "Render/GUI/Texture.h"
#include "../../Utils/Math/Vector.h"
#include "DataStructures/CArrayList.h"

struct ArcInfo {
    int start;
    int end;
    int corner;
};

static struct ArcInfo Mesh_triangulate(const Vec2f corner, const float radius, GuiVertex **aVertices, int **aIndices, float startAngle, float endAngle, int numTriangles) {
    const float radStep = (endAngle - startAngle)/(float)numTriangles;
    const int cornerIndex = arrLen(*aVertices);

    arrPush(*aVertices, (GuiVertex){.pos = corner});

    const int startIndex = cornerIndex + 1;

    arrPush(*aVertices, (GuiVertex){
        .pos = (Vec2f){
            corner.x + cosf(startAngle)*radius,
            corner.y + sinf(startAngle)*radius,
        }
    });

    const int lim = (radius > 0 ? numTriangles : 1);

    for (int i = 1 ; i <= lim; i++) {
        const int idx = arrLen(*aVertices);

        arrPush(*aVertices, (GuiVertex){
            .pos = (Vec2f){
                corner.x + cosf(startAngle + i * radStep)*radius,
                corner.y + sinf(startAngle + i * radStep)*radius,
            }
        });

        const int prev = idx - 1;
        const int curr = idx;

        arrPush(*aIndices, cornerIndex);
        arrPush(*aIndices, prev);
        arrPush(*aIndices, curr);
    }
    return (struct ArcInfo){
        .start = startIndex,
        .end = arrLen(*aVertices) - 1,
        .corner = cornerIndex
    };
}

static void Mesh_connectFans(const struct ArcInfo* a1, const struct ArcInfo* a2, int **aIndices) {
    arrPush(*aIndices, a1->end);
    arrPush(*aIndices, a1->corner);
    arrPush(*aIndices, a2->start);

    arrPush(*aIndices, a2->corner);
    arrPush(*aIndices, a2->start);
    arrPush(*aIndices, a1->corner);
}

void Mesh_generateRoundedCorner(const Element* element, GuiVertex** aVertices, int** aIndices, ssize_t id) {
    const float width = element->dims.worldWidth;
    const float height = element->dims.worldHeight;
    const float radius = min((float)element->dims.cornerRadius, min(width, height) * 0.5f);

    constexpr float r90 = (float)M_PI * 0.5f;
    constexpr int numTriangles = 12;

    const int start = arrLen(*aVertices);

    const struct ArcInfo tl = Mesh_triangulate(
        (Vec2f){radius, radius},
        radius,
        aVertices,
        aIndices,
        2*r90,
        3*r90,
        numTriangles
    );

    const struct ArcInfo tr = Mesh_triangulate(
        (Vec2f){width-radius, radius},
        radius,
        aVertices,
        aIndices,
        3*r90,
        4*r90,
        numTriangles
    );

    const struct ArcInfo br = Mesh_triangulate(
        (Vec2f){width-radius, height-radius},
        radius,
        aVertices,
        aIndices,
        0,
        r90,
        numTriangles
    );

    const struct ArcInfo bl = Mesh_triangulate(
        (Vec2f){radius, height-radius},
        radius,
        aVertices,
        aIndices,
        r90,
        2*r90,
        numTriangles
    );
    Mesh_connectFans(&tl, &tr, aIndices);
    Mesh_connectFans(&tr, &br, aIndices);
    Mesh_connectFans(&br, &bl, aIndices);
    Mesh_connectFans(&bl, &tl, aIndices);

    arrPush(*aIndices, tl.corner);
    arrPush(*aIndices, bl.corner);
    arrPush(*aIndices, tr.corner);

    arrPush(*aIndices, tr.corner);
    arrPush(*aIndices, bl.corner);
    arrPush(*aIndices, br.corner);

    const Vec2f uv0 = element->visuals.texture.uv0;
    const Vec2f uv1 = element->visuals.texture.uv1;

    const int lim = arrLen(*aVertices);

    for (int i = start; i < lim; i++) {
        GuiVertex* v = &(*aVertices)[i];

        const float uNorm = v->pos.x / width;
        const float vNorm = v->pos.y / height;

        v->uv = (Vec2f){
            uv0.x * (1.0f - uNorm) + uv1.x * uNorm,
            uv0.y * (1.0f - vNorm) + uv1.y * vNorm
        };
        v->ID = id;
        v->bufferBinding = 0;
    }
}

void Mesh_customQuad(const Element* element, const Vec2f pos, const Vec2f dims, Vec4f color, GuiVertex **aVertices, int **aIndices, MeshInstanceData** aAdditional) {
    const int id = arrLen(*aAdditional);
    const int start = arrLen(*aVertices);

    arrPush(*aVertices, (GuiVertex){.ID = id, .pos = pos});
    arrPush(*aVertices, (GuiVertex){.ID = id, .pos = {pos.x, pos.y + dims.y}});
    arrPush(*aVertices, (GuiVertex){.ID = id, .pos = {pos.x + dims.x, pos.y + dims.y}});
    arrPush(*aVertices, (GuiVertex){.ID = id, .pos = {pos.x + dims.x, pos.y}});

    arrPush(*aIndices, start);
    arrPush(*aIndices, start+1);
    arrPush(*aIndices, start+2);

    arrPush(*aIndices, start);
    arrPush(*aIndices, start+2);
    arrPush(*aIndices, start+3);

    arrPush(*aAdditional, (MeshInstanceData){.color = color, .atlasID = 0, .ownerID = element->handle.ID});
}
