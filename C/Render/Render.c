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

    //initBatchedRendering();
    loadDefaultQuadMesh();

    //ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    //ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    //ComputeShader_update(&renderer->computeShader, graphingFunction);

    Shader_createUniform(&renderer->guiShader, "hasTexture");
    Shader_createUniform(&renderer->guiShader, "width");
    Shader_createUniform(&renderer->guiShader, "height");
    Shader_createUniform(&renderer->guiShader, "screenWidth");
    Shader_createUniform(&renderer->guiShader, "screenHeight");
    Shader_createUniform(&renderer->guiShader, "positionObject");
    Shader_createUniform(&renderer->guiShader, "texture_sampler");
    Shader_createUniform(&renderer->guiShader, "color");
    Shader_createUniform(&renderer->guiShader, "transparency");
    Shader_createUniform(&renderer->guiShader, "brightness");

    //Shader_createUniform(&renderer->batched_guiShader, "screenWidth");
    //Shader_createUniform(&renderer->batched_guiShader, "screenHeight");
    //Shader_createUniform(&renderer->batched_guiShader, "atlasSampler");

    Shader_createUniform(&renderer->textShader, "fontAtlas");
    Shader_createUniform(&renderer->textShader, "screenWidth");
    Shader_createUniform(&renderer->textShader, "screenHeight");
    Shader_createUniform(&renderer->textShader, "color");
}

void Renderer_render(const Renderer *renderer) {
    ComputeShader_run(&renderer->computeShader);

    glClear(GL_COLOR_BUFFER_BIT);
    //#define DEBUG
#ifdef DEBUG
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    Shaders.bind(&renderer->guiShader);
    setUniform(&renderer->guiShader, ("screenWidth"), (float) renderer->screenWidth);
    setUniform(&renderer->guiShader, ("screenHeight"), (float) renderer->screenHeight);

    const Element* guiRoot = renderer->guiRoot;

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        renderElementsRecursively(guiRoot->childElements.content[i], renderer);
    }

    glfwSwapBuffers(renderer->window);
    Shaders.unbind();
}

void GUI_render(const Renderer *renderer) {
    glDisable(GL_DEPTH_TEST);  // GUI is in screen-space, depth irrelevant
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef DEBUG
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    Shaders.bind(&renderer->guiShader);
    setUniform(&renderer->guiShader, ("screenWidth"), (float) renderer->screenWidth);
    setUniform(&renderer->guiShader, ("screenHeight"), (float) renderer->screenHeight);

    const Element* guiRoot = renderer->guiRoot;

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        renderElementsRecursively(guiRoot->childElements.content[i], renderer);
    }

    Shaders.unbind();
}

void renderElementsRecursively(Element* element, const Renderer* renderer) {
    if (element == nullptr || !element->flags.isActive) return;
    const Shader* shader = &renderer->guiShader;

    setUniform(shader, "width", (float)element->actualWidth);
    setUniform(shader, "height", (float)element->actualHeight);
    setUniform(shader, "transparency", (float)1-element->transparency);
    setUniform(shader, "brightness", (float)element->brightness);

    setUniform(shader, "hasTexture", element->simpleTexture != NULL);

    setUniform(shader, "positionObject", toVec2f(element->worldPos));

    setUniform(shader, "texture_sampler", 0);

    setUniform(shader, "color", element->color);

    glActiveTexture(GL_TEXTURE0);

    if (element->simpleTexture != NULL) {
        glBindTexture(GL_TEXTURE_2D, element->simpleTexture->textureId);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Mesh_render(&element->Mesh);

    if (element->textElement.hasText) {
        renderTextRetained(renderer, element);
        Shaders.bind(&renderer->guiShader);
    }

    for (int i = 0; i < element->childElements.size; i++) {
        Element* childElement = element->childElements.content[i];
        renderElementsRecursively(childElement, renderer);
    }
}

void Renderer_render2(const Renderer *renderer) {
    ComputeShader_run(&renderer->computeShader);
    static GuiVertex vertices[MAX_GUI_VERTICES];
    int numVertices = 0;

    Element* textElements[1024];
    int numTextElements = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

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

    glfwSwapBuffers(renderer->window);
    Shaders.unbind();
}

GuiVertex getUpdatedVertex(const GuiVertex *vertex, int width, int height, Vec2i pos) {
    return (GuiVertex){
        .hasTexture = vertex->hasTexture,
        .color = vertex->color,
        .brightness = vertex->brightness,
        .pos = {
            vertex->pos.x * (float) width + (float) pos.x,
            vertex->pos.y * (float) height + (float) pos.y
        },
        .uv = vertex->uv
    };
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
        .render = Renderer_render,
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
        .render = Renderer_render,
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
    quadMesh = Mesh_loadSimpleQuad();
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}


