//
// Created by Stefan on 10.10.2025.
//
#include "../../Dependencies/include/glad/gl.h"
#include "Render.h"

#include <bemapiset.h>
#include <math.h>

#include "Engine.h"
#include "../Utils/Vector.h"
#include "GUI/CallbackFunctions.h"
#include <../../Dependencies/include/stb/stb_truetype.h>



#define MAX_GUI_VERTICES 16384

typedef struct {
    GLuint VAO;
    GLuint VBO;
} GraphicsData;

Mesh quadMesh;

void loadDefaultQuadMesh();
void renderElementsRecursively(Element* element, const Renderer* renderer);
void renderBatchedQuads(GLuint atlasId, const GuiVertex *vertices, int length);
void initBatchedRendering();

void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *index, Element **textElements, int *
                      numTextElements);

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
        return NULL;
    }
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return window;
}

void GUIRenderer_init(Renderer *renderer) {

    initBatchedRendering();
    loadDefaultQuadMesh();

    //ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    //ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    //ComputeShader_update(&renderer->computeShader, graphingFunction);

    Shader_createUniform(&renderer->batched_guiShader, "screenWidth");
    Shader_createUniform(&renderer->batched_guiShader, "screenHeight");
    Shader_createUniform(&renderer->batched_guiShader, "atlasSampler");

    Shader_createUniform(&renderer->textShader, "fontAtlas");
    Shader_createUniform(&renderer->textShader, "screenWidth");
    Shader_createUniform(&renderer->textShader, "screenHeight");
    Shader_createUniform(&renderer->textShader, "color");
}

void Renderer_render2(const Renderer *renderer) {
    static GuiVertex vertices[MAX_GUI_VERTICES];
    int numVertices = 0;

    Element* textElements[1024];
    int numTextElements = 0;
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //#define DEBUG
#ifdef DEBUG
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    Shaders.bind(&renderer->batched_guiShader);
    setUniform(&renderer->batched_guiShader, ("screenWidth"), (float) renderer->screenWidth);
    setUniform(&renderer->batched_guiShader, ("screenHeight"), (float) renderer->screenHeight);

    setUniform(&renderer->batched_guiShader, "atlasSampler", 0);

    const Element* guiRoot = renderer->guiRoot;

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        accumulateMeshes(guiRoot->childElements.content[i],
                         renderer,
                         vertices,
                         &numVertices,
                         textElements,
                         &numTextElements
        );
    }
    renderBatchedQuads(renderer->atlasId, vertices, numVertices);

    for (int i = 0; i < numTextElements; i++) {
        renderTextRetained(renderer, textElements[i]);
    }

    Shaders.unbind();
}

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

void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *index, Element **textElements, int *numTextElements) {
    if (element == NULL || !element->flags.isActive) return;

    if (element->textElement.hasText) textElements[(*numTextElements)++] = element;

    for (int i = 0; i < element->guiMesh.length; i++) {
        vertices[(*index)++] = getUpdatedVertex(&element->guiMesh.vertices[i], element->actualWidth, element->actualHeight, element->worldPos);
    }

    for (int i = 0; i < element->childElements.size; i++) {
        Element* childElement = element->childElements.content[i];
        accumulateMeshes(childElement, renderer, vertices, index, textElements, numTextElements);
    }
}

static GraphicsData graphicsData;
[[todo]]
void initBatchedRendering() {

    glGenVertexArrays(1, &graphicsData.VAO);
    glGenBuffers(1, &graphicsData.VBO);

    glBindVertexArray(graphicsData.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);

    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, pos));

    glEnableVertexAttribArray(1); //uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, uv));

    glEnableVertexAttribArray(2); //color
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, color));

    glEnableVertexAttribArray(3); //brightness
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, brightness));

    glEnableVertexAttribArray(4); //texture flag
    glVertexAttribIPointer(4, 1, GL_INT,
                          sizeof(GuiVertex), (void*)offsetof(GuiVertex, hasTexture));

    glBufferData(GL_ARRAY_BUFFER,
             MAX_GUI_VERTICES * sizeof(GuiVertex),
             NULL,
             GL_DYNAMIC_DRAW);
}
[[todo]]
void renderBatchedQuads(const GLuint atlasId, const GuiVertex *vertices, const int length) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasId);
    glBindVertexArray(graphicsData.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(GuiVertex) * length, vertices);

    glDrawArrays(GL_TRIANGLES, 0, length);
}

Vec2i updateLayout(Element* element, const Vec2i parentCursor, const Vec2i parentPos, const Font* font) {
    if (!element || !element->flags.isActive) return (Vec2i){0,0};
    if (element->reset) element->reset(element);

    if (element->positionMode == POS_FIT) {
        element->pos.x = parentCursor.x;
        element->pos.y = parentCursor.y;
    }
    element->worldPos.x = element->pos.x + parentPos.x;
    element->worldPos.y = element->pos.y + parentPos.y;

    element->actualWidth = element->width;
    element->actualHeight = element->height;

    Vec2i cursor = (Vec2i){element->padding.left, element->padding.up};
    const LayoutDirection layoutDirection = element->layoutDirection;

    int extendRight = element->padding.left;
    int extendDown = element->padding.up;

    const Padding padding = element->padding;

    if (element->textElement.hasText) {
        const int textW = padding.left + (int)element->textElement.width + padding.right;
        const int textH = padding.up + (int)((float)(font->maxCharHeight) * element->textElement.textScale) + padding.down;

        element->actualWidth  = max(element->actualWidth ,  textW);
        element->actualHeight = max(element->actualHeight, textH);
    }

    Element* elements[16];
    int numGrowingElements = 0;

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];

        Vec2i childDimensions = updateLayout(child, cursor, element->worldPos, font);

        if (element->flags.fixedHeight && cursor.y + childDimensions.y > element->height) {
            cursor.y = padding.up;
            cursor.x = extendRight + element->childGap;
            childDimensions = updateLayout(child, cursor, element->worldPos, font);
        }
        if (element->flags.fixedWidth && cursor.x + childDimensions.x > element->width) {
            cursor.y = extendDown + element->childGap;
            cursor.x = padding.left;
            childDimensions = updateLayout(child, cursor, element->worldPos, font);
        }

        extendRight = max(extendRight, child->pos.x + childDimensions.x);
        extendDown  = max(extendDown,  child->pos.y + childDimensions.y);

        if (child->positionMode == POS_FIT) {
            if (layoutDirection == L_down) cursor.y += childDimensions.y + element->childGap;
            else if (layoutDirection == L_right) cursor.x += childDimensions.x + element->childGap;
        } else {
            if (layoutDirection == L_down) cursor.y = extendDown + element->childGap;
            else if (layoutDirection == L_right) cursor.x = extendRight + element->childGap;
        }

        if (child->flags.wantGrowHorizontal || child->flags.wantGrowVertical) elements[numGrowingElements++] = child;
    }

    element->actualWidth = max(element->actualWidth, extendRight + padding.right);
    element->actualHeight = max(element->actualHeight, extendDown + padding.down);

    //TODO fix
    for (int i = 0; i < numGrowingElements; i++) {
        Element* child = elements[i];
        if (child->flags.wantGrowHorizontal) child->actualWidth = element->actualWidth - padding.left - padding.right;
        if (child->flags.wantGrowVertical) child->actualHeight = element->actualHeight - padding.up - padding.down;
    }

    return (Vec2i){element->actualWidth, element->actualHeight};
}

inline bool isMousePressed(GLFWwindow* window, const int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

Renderer newRenderer(const int width, const int height, const char* name, char *fontFile) {
    return (Renderer){
        .guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag"),
        .batched_guiShader = newShader("GuiRender.vert", "GuiRender.frag"),
        .textShader = newShader("TextRender.vert", "TextRender.frag"),
        .window = initWindow(width, height, name),
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas(fontFile),
        .basicQuadMesh = Mesh_loadSimpleQuad(),
        .defaultClick = nullptr,
        .guiRoot = createRootElement()
    };
}

Renderer newGUIRenderer(GLFWwindow* window, const int width, const int height, char *fontFile) {
    return (Renderer){
        .guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag"),
        .batched_guiShader = newShader("GuiRender.vert", "GuiRender.frag"),
        .textShader = newShader("TextRender.vert", "TextRender.frag"),
        .window = window,
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas(fontFile),
        .basicQuadMesh = Mesh_loadSimpleQuad(),
        .defaultClick = nullptr,
        .guiRoot = createRootElement()
    };
}

Element* createRootElement() {
    return newElement((Mesh){}, (Vec2i){}, 0, 0, nullptr);
}

void loadDefaultQuadMesh() {
    //quadMesh = Mesh_loadSimpleQuad();
    quadMesh = Mesh_loadRoundedCornerMesh2(0.1f);
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}


