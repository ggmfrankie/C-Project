//
// Created by Stefan on 10.10.2025.
//
#include "../../../Dependencies/include/glad/gl.h"
#include "Render.h"

#include <bemapiset.h>
#include <math.h>

#include "Batcher.h"
#include "../Engine.h"
#include "../../Utils/Vector.h"
#include "../GUI/CallbackFunctions.h"

void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *vt, int *indices, int *id);

Element* createRootElement();

GLFWwindow* initWindow(const int width, const int height, const char* name) {
    if (!glfwInit()) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return window;
}

void GUIRenderer_init(Renderer *renderer) {
    //ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    //ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    //ComputeShader_update(&renderer->computeShader, graphingFunction);

    initBatchedRendering();
    Shader_createUniform(&renderer->guiShader, "screenWidth");
    Shader_createUniform(&renderer->guiShader, "screenHeight");
}


void beginScissor(const Element* e, const int screenHeight) {
    glEnable(GL_SCISSOR_TEST);

    const int x = e->dims.worldPos.x;
    const int y = screenHeight - (e->dims.worldPos.y + e->dims.actualHeight);
    const int w = e->dims.actualWidth;
    const int h = e->dims.actualHeight;

    glScissor(x, y, w, h);
}

void endScissor() {
    glDisable(GL_SCISSOR_TEST);
}

void Renderer_render2(const Renderer *renderer) {
    static GuiVertex vertices[MAX_GUI_VERTICES];
    int numVertices = 0;
    static int indices[MAX_GUI_VERTICES];
    int numIndices = 0;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //#define DEBUG
#ifdef DEBUG
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    Shaders.bind(&renderer->guiShader);
    glEnable(GL_MULTISAMPLE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->texAtlas.ID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderer->font.fontAtlas.ID);

    setUniform(&renderer->guiShader, "screenWidth", (float) renderer->screenWidth);
    setUniform(&renderer->guiShader, "screenHeight", (float) renderer->screenHeight);

    const Element* guiRoot = renderer->guiRoot;

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        accumulateMeshes(guiRoot->childElements.content[i],
                         renderer,
                         vertices,
                         &numVertices,
                         indices,
                         &numIndices
        );
    }
    uploadBatchedQuads(vertices, numVertices, indices, numIndices);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDisable(GL_MULTISAMPLE);
    Shaders.unbind();
}

void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *vt, int *indices, int *id) {
    if (element == NULL || !element->flags.isActive) return;

    beginScissor(element, renderer->screenHeight);

    if (!element->flags.invisible) {
        element->generateMesh(element, vertices, vt, indices, id);
    }
    uploadElementData(element);
    accumulateTextQuads(element, vertices, vt, indices, id, &renderer->font);

    for (int i = 0; i < element->childElements.size; i++) {
        Element* childElement = element->childElements.content[i];
        accumulateMeshes(childElement, renderer, vertices, vt, indices, id);
    }
    endScissor();
}

Vec2i updateLayout(Element* element, const Vec2i parentCursor, const Vec2i parentPos, const Font* font) {
    if (!element || !element->flags.isActive) return (Vec2i){0,0};
    const auto cb = &element->callbacks;
    const auto dims = &element->dims;
    if (cb->reset) cb->reset(element);

    if (element->positionMode == POS_FIT) {
        dims->pos.x = parentCursor.x;
        dims->pos.y = parentCursor.y;
    }
    dims->worldPos.x = dims->pos.x + parentPos.x;
    dims->worldPos.y = dims->pos.y + parentPos.y;

    dims->actualWidth = dims->width;
    dims->actualHeight = dims->height;

    auto cursor = (Vec2i){element->padding.left, element->padding.up};
    const LayoutDirection layoutDirection = element->layoutDirection;

    int extendRight = element->padding.left;
    int extendDown = element->padding.up;

    const Padding padding = element->padding;

    if (element->textElement.hasText) {
        const int textW = padding.left + (int)element->textElement.width + padding.right;
        const int textH = padding.up + (int)((float)(font->maxCharHeight) * element->textElement.textScale) + padding.down;

        dims->actualWidth  = max(dims->actualWidth ,  textW);
        dims->actualHeight = max(dims->actualHeight, textH);
    }

    Element* elements[16];
    int numGrowingElements = 0;

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];

        Vec2i childDimensions = updateLayout(child, cursor, dims->worldPos, font);

        if (element->flags.fixedHeight && cursor.y + childDimensions.y > dims->height) {
            cursor.y = padding.up;
            cursor.x = extendRight + element->childGap;
            childDimensions = updateLayout(child, cursor, dims->worldPos, font);
        }
        if (element->flags.fixedWidth && cursor.x + childDimensions.x > dims->width) {
            cursor.y = extendDown + element->childGap;
            cursor.x = padding.left;
            childDimensions = updateLayout(child, cursor, dims->worldPos, font);
        }

        extendRight = max(extendRight, child->dims.pos.x + childDimensions.x);
        extendDown  = max(extendDown,  child->dims.pos.y + childDimensions.y);

        if (child->positionMode == POS_FIT) {
            if (layoutDirection == L_down) cursor.y += childDimensions.y + element->childGap;
            else if (layoutDirection == L_right) cursor.x += childDimensions.x + element->childGap;
        } else {
            if (layoutDirection == L_down) cursor.y = extendDown + element->childGap;
            else if (layoutDirection == L_right) cursor.x = extendRight + element->childGap;
        }

        if (child->flags.wantGrowHorizontal || child->flags.wantGrowVertical) elements[numGrowingElements++] = child;
    }

    dims->actualWidth = max(dims->actualWidth, extendRight + padding.right);
    dims->actualHeight = max(dims->actualHeight, extendDown + padding.down);

    //TODO fix
    for (int i = 0; i < numGrowingElements; i++) {
        Element* child = elements[i];
        if (child->flags.wantGrowHorizontal) child->dims.actualWidth = dims->actualWidth - padding.left - padding.right;
        if (child->flags.wantGrowVertical) child->dims.actualHeight = dims->actualHeight - padding.up - padding.down;
    }

    return (Vec2i){dims->actualWidth, dims->actualHeight};
}

inline bool isMousePressed(GLFWwindow* window, const int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

Renderer newGUIRenderer(GLFWwindow* window, const int width, const int height, char *fontFile) {
    return (Renderer){
        .guiShader = newShader("GuiRender.vert", "GuiRender.frag"),
        .window = window,
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas(fontFile),
        .guiRoot = createRootElement(),
        .texAtlas = loadTextureAtlas(2048 , 2048)
    };
}

Element* createRootElement() {
    return newElement((Vec2i){}, 0, 0);
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}


