//
// Created by ertls on 11.02.2026.
//

#ifndef C_BATCHEDRENDERING_H
#define C_BATCHEDRENDERING_H
#include "../../Utils/Vector.h"
#include "glad/gl.h"

typedef struct {
    Vec2f pos;
    Vec2f uv;

    Vec4f color;
    float brightness;

} GuiVertex;

typedef struct {
    GLuint VAO;
    GLuint VBO;
} GraphicsData;

#endif //C_BATCHEDRENDERING_H