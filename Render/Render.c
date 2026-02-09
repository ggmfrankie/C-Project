//
// Created by Stefan on 10.10.2025.
//
#include "glad/gl.h"
#include "Render.h"

#include <bemapiset.h>
#include <math.h>

#include "Engine.h"
#include "../Utils/Vector.h"
#include "GUI/CallbackFunctions.h"
#include <stb/stb_truetype.h>

Mesh quadMesh;

void loadDefaultQuadMesh();
void resizeCallback(GLFWwindow *window, int width, int height);
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void renderElementsRecursively(Element* element, const Renderer* renderer);

Element createRootElement();

GLFWwindow* initWindow(const int width, const int height, const char* name) {
    if (!glfwInit()) {
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
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

void Renderer_init(Renderer *renderer) {
    glfwSetWindowUserPointer(renderer->window, renderer);

    const GLFWframebuffersizefun callbackFun = resizeCallback;
    glfwSetFramebufferSizeCallback(renderer->window, callbackFun);
    glfwSetCursorPosCallback(renderer->window, cursorPositionCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadDefaultQuadMesh();

    ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    ComputeShader_update(&renderer->computeShader, graphingFunction);

    Shader_createUniform(&renderer->guiShader, "hasTexture");
    Shader_createUniform(&renderer->guiShader, "state");
    Shader_createUniform(&renderer->guiShader, "width");
    Shader_createUniform(&renderer->guiShader, "height");
    Shader_createUniform(&renderer->guiShader, "screenWidth");
    Shader_createUniform(&renderer->guiShader, "screenHeight");
    Shader_createUniform(&renderer->guiShader, "positionObject");
    Shader_createUniform(&renderer->guiShader, "texture_sampler");
    Shader_createUniform(&renderer->guiShader, "color");

    Shader_createUniform(&renderer->guiShader, "transformTexCoords");
    Shader_createUniform(&renderer->guiShader, "texPosStart");
    Shader_createUniform(&renderer->guiShader, "texPosEnd");

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

    const Element* guiRoot = &renderer->guiRoot;

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        renderElementsRecursively(guiRoot->childElements.content[i], renderer);
    }

    glfwSwapBuffers(renderer->window);
    Shaders.unbind();
}

void renderElementsRecursively(Element* element, const Renderer* renderer) {
    if (element == NULL || !element->flags.bits.isActive) return;
    const Shader* shader = &renderer->guiShader;

    setUniform(shader, "width", (float)element->actualWidth);
    setUniform(shader, "height", (float)element->actualHeight);

    setUniform(shader, "hasTexture", element->texture != NULL);
    setUniform(shader, "state", (int)element->state);

    setUniform(shader, "positionObject", toVec2f(element->worldPos));
    setUniform(shader, "transformTexCoords", 0);

    setUniform(shader, "texture_sampler", 0);

    setUniform(shader, "color", element->color);

    glActiveTexture(GL_TEXTURE0);

    if (element->texture != NULL) {
        glBindTexture(GL_TEXTURE_2D, element->texture->textureId);
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

[[deprecated]]
Vec2i updateLayout(Element *element, const Vec2i parentPos, const Font *font, const int verticalOffset) {
    if (!element || !element->flags.bits.isActive) return (Vec2i){0,0};
    if (element->reset) element->reset(element);

    if (element->positionMode == POS_FIT) {
        element->pos.x = 0;
        element->pos.y = verticalOffset;
    }

    element->worldPos.x = parentPos.x + element->pos.x;
    element->worldPos.y = parentPos.y + element->pos.y;

    const Vec2i contentOrigin = {
        .x = element->worldPos.x + element->padding.left,
        .y = element->worldPos.y + element->padding.up
    };

    int realWidth = element->width;
    int realHeight = element->height;

    const Padding* padding = &element->padding;

    if (element->textElement.hasText) {
        const Vec2i textSize = measureElementText(font, &element->textElement);
        const int textW = padding->left + textSize.x + padding->right;
        const int textH = padding->up + textSize.y + padding->down;

        realWidth  = max(realWidth,  textW);
        realHeight = max(realHeight, textH);
    }

    int accumulatedHeight = 0;
    int maxChildWidth = 0;
    int maxChildHeight = 0;

    Vec2i contentExtend = (Vec2i){0,0};

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];
        const Vec2i childDimensions = updateLayout(child, contentOrigin, font, accumulatedHeight);

        const int childWidth = child->pos.x + childDimensions.x;
        const int childHeight = child->pos.y + childDimensions.y;

        maxChildWidth = max(maxChildWidth, childWidth);
        maxChildHeight = max(maxChildHeight, childHeight);

        contentExtend.x = max(contentExtend.x, childWidth);
        contentExtend.y = max(contentExtend.y, childHeight);

        if (child->positionMode == POS_FIT) {
            accumulatedHeight += childDimensions.y + element->childGap;
        } else {
            accumulatedHeight = contentExtend.y + element->childGap;
        }
    }

    realWidth  = max(realWidth,  padding->left + maxChildWidth  + padding->right);
    realHeight = max(realHeight, padding->up   + maxChildHeight + padding->down);

    element->actualWidth = realWidth;
    element->actualHeight = realHeight;

    return (Vec2i){realWidth, realHeight};
}

[[deprecated]]
Vec2i updateLayout2(Element *element, const Vec2i parentPos, const Font *font, const Vec2i offset, LayoutDirection layoutDirection) {
    if (!element || !element->flags.bits.isActive) return (Vec2i){0,0};
    if (element->reset) element->reset(element);

    if (element->positionMode == POS_FIT) {
        if (layoutDirection == L_down) {
            element->pos.x = 0;
            element->pos.y = offset.y;
        } else if (layoutDirection == L_right) {
            element->pos.x = offset.x;
            element->pos.y = 0;
        }
    }

    element->worldPos.x = parentPos.x + element->pos.x;
    element->worldPos.y = parentPos.y + element->pos.y;

    Vec2i contentOrigin = {
        .x = element->worldPos.x + element->padding.left,
        .y = element->worldPos.y + element->padding.up
    };

    int realWidth = element->width;
    int realHeight = element->height;

    const Padding padding = element->padding;

    if (element->textElement.hasText) {
        const Vec2i textSize = measureElementText(font, &element->textElement);
        const int textW = padding.left + textSize.x + padding.right;
        const int textH = padding.up + textSize.y + padding.down;

        realWidth  = max(realWidth,  textW);
        realHeight = max(realHeight, textH);
    }


    Vec2i cursor = {};

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];

        const Vec2i childDimensions = updateLayout2(child, contentOrigin, font, cursor, element->layoutDirection);

        const int childWidth = child->pos.x + childDimensions.x;
        const int childHeight = child->pos.y + childDimensions.y;

        cursor.x = max(cursor.x, childWidth + ((child->positionMode == POS_FIT) ? element->childGap : 0));
        cursor.y = max(cursor.y, childHeight + ((child->positionMode == POS_FIT) ? element->childGap : 0));

        if (i + 1 < element->childElements.size) {
            if (element->positionMode == POS_FIT) {
                if (layoutDirection == L_down) cursor.y += element->childGap;
                else                           cursor.x += element->childGap;
            }
        }
    }

    realWidth  = max(realWidth,  padding.left + cursor.x  + padding.right);
    realHeight = max(realHeight, padding.up   + cursor.y + padding.down);

    element->actualWidth = realWidth;
    element->actualHeight = realHeight;

    return (Vec2i){realWidth, realHeight};
}

Vec2i updateLayout3(Element* element, const Vec2i parentCursor, const Vec2i parentPos, const Font* font) {
    if (!element || !element->flags.bits.isActive) return (Vec2i){0,0};
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
        //const Vec2i textSize = measureElementText(font, &element->textElement);
        const int textW = padding.left + (int)element->textElement.width + padding.right;
        const int textH = padding.up + (int)((float)(font->maxCharHeight) * element->textElement.textScale) + padding.down;

        element->actualWidth  = max(element->actualWidth ,  textW);
        element->actualHeight = max(element->actualHeight, textH);
    }

    Element* elements[16];
    int numGrowingElements = 0;

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];

        Vec2i childDimensions = updateLayout3(child, cursor, element->worldPos, font);

        if (element->flags.bits.fixedHeight && cursor.y + childDimensions.y > element->height) {
            cursor.y = padding.up;
            cursor.x = extendRight + element->childGap;
            childDimensions = updateLayout3(child, cursor, element->worldPos, font);
        }
        if (element->flags.bits.fixedWidth && cursor.x + childDimensions.x > element->width) {
            cursor.y = extendDown + element->childGap;
            cursor.x = padding.left;
            childDimensions = updateLayout3(child, cursor, element->worldPos, font);
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

        if (child->flags.bits.wantGrowHorizontal || child->flags.bits.wantGrowVertical) elements[numGrowingElements++] = child;
    }

    element->actualWidth = max(element->actualWidth, extendRight + padding.right);
    element->actualHeight = max(element->actualHeight, extendDown + padding.down);

    //TODO fix
    for (int i = 0; i < numGrowingElements; i++) {
        Element* child = elements[i];
        if (child->flags.bits.wantGrowHorizontal) child->actualWidth = element->actualWidth - padding.left - padding.right;
        if (child->flags.bits.wantGrowVertical) child->actualHeight = element->actualHeight - padding.up - padding.down;
    }

    return (Vec2i){element->actualWidth, element->actualHeight};
}

void resizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);

    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->screenWidth = width;
    renderer->screenHeight = height;
}

void cursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos) {
    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->mousePos.x = (int)xPos;
    renderer->mousePos.y = (int)yPos;
}

inline bool isMousePressed(GLFWwindow* window, const int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

Renderer newRenderer(const int width, const int height, const char* name) {
    return (Renderer){
        .guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag"),
        .textShader = newShader("TextRender.vert", "TextRender.frag"),
        .otherShaders = (OtherShaders){0, {}},
        .computeShader = 0,
        .window = initWindow(width, height, name),
        .render = Renderer_render,
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas("ARIAL.TTF"),
        .basicQuadMesh = Mesh_loadSimpleQuad(),
        .defaultClick = NULL,
        .guiRoot = createRootElement()
    };
}

Element createRootElement() {
    return newElement((Mesh){}, (Vec2i){0}, 0, 0, NULL);
}

void loadDefaultQuadMesh() {
    quadMesh = Mesh_loadSimpleQuad();
}


void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

