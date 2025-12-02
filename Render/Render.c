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

void loadDefaultQuadMesh();
void resizeCallback(GLFWwindow *window, int width, int height);
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void renderElementsRecursively(Element* element, const Renderer* renderer);
Vec2f measureText(const Renderer* renderer, const TextElement* textElement);

Vec2f updateLayout(Element *element, Vec2f parentPos, const Renderer* renderer);
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
}

void Renderer_render(Renderer *renderer) {
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

    Element* guiRoot = &renderer->guiRoot;

    updateLayout(guiRoot, (Vec2f){0.0f, 0.0f}, renderer);

    for (int i = 0; i < guiRoot->childElements.size; i++) {
        renderElementsRecursively(guiRoot->childElements.content[i], renderer);
    }
    /*
    for (size_t i = 0; i < renderer->elements->size; i++) {
        Element *element = Element_ListGet_ptr(renderer->elements, i);
        if (element == NULL) break;

        const Shader *shader = &renderer->guiShader;

        setUniform(shader, ("width"), element->actualWidth);
        setUniform(shader, ("height"), element->actualHeight);

        setUniform(shader, ("hasTexture"), 1);
        setUniform(shader, ("state"), (int)element->state);

        setUniform(shader, ("positionObject"), element->worldPos);
        setUniform(shader, ("transformTexCoords"), 0);

        glActiveTexture(GL_TEXTURE0);
        if (element->texture != NULL) {
            glBindTexture(GL_TEXTURE_2D, element->texture->textureId);
        }

        setUniform(shader, ("texture_sampler"), 0);
        Mesh_render(&element->Mesh);

        if (element->hasText) {
            renderText(renderer, element);
        }
    }
    */

    glfwSwapBuffers(renderer->window);
    Shaders.unbind();
}

void renderElementsRecursively(Element* element, const Renderer* renderer) {
    if (element == NULL) return;

    const Shader* shader = &renderer->guiShader;

    setUniform(shader, ("width"), element->actualWidth);
    setUniform(shader, ("height"), element->actualHeight);

    setUniform(shader, ("hasTexture"), 1);
    setUniform(shader, ("state"), (int)element->state);

    setUniform(shader, ("positionObject"), element->worldPos);
    setUniform(shader, ("transformTexCoords"), 0);

    glActiveTexture(GL_TEXTURE0);
    if (element->texture != NULL) {
        glBindTexture(GL_TEXTURE_2D, element->texture->textureId);
    } else {
        setUniform(shader, ("hasTexture"), 0);
        setUniform(shader, "color", element->color);
    }

    setUniform(shader, ("texture_sampler"), 0);
    Mesh_render(&element->Mesh);

    if (element->hasText) {
        renderText(renderer, element);
    }

    for (int i = 0; i < element->childElements.size; i++) {
        Element* childElement = element->childElements.content[i];
        renderElementsRecursively(childElement, renderer);
    }
}

Vec2f updateLayout(Element *element, const Vec2f parentPos, const Renderer* renderer) {
    if (!element) return (Vec2f){0,0};

    element->worldPos.x = parentPos.x + max(element->pos.x, element->parentElement ? element->parentElement->padding.left : 0);
    element->worldPos.y = parentPos.y + max(element->pos.y, element->parentElement ? element->parentElement->padding.up : 0);

    float realWidth = element->width;
    float realHeight = element->height;

    const Padding* padding = &element->padding;

    if (element->hasText) {
        const Vec2f textSize = measureText(renderer, &element->textElement);
        const float textW = padding->left + textSize.x + element->textElement.offset.x + padding->right;
        const float textH = padding->up + textSize.y + element->textElement.offset.y + padding->down;

        realWidth  = max(realWidth,  textW);
        realHeight = max(realHeight, textH);
    }

    for (int i = 0; i < element->childElements.size; i++) {
        Element *child = element->childElements.content[i];
        const Vec2f childDimensions = updateLayout(child, element->worldPos, renderer);

        const float childWidth = padding->left + child->pos.x + childDimensions.x + padding->right;
        const float childHeight = padding->up + child->pos.y + childDimensions.y + padding->down;

        realWidth = max(realWidth, childWidth);
        realHeight = max(realHeight, childHeight);
        //realHeight += childDimensions.y;
    }

    if (element->autoFit) {
        element->actualWidth = realWidth;
        element->actualHeight = realHeight;
    }
    return (Vec2f){realWidth, realHeight};
}

Vec2f measureText(const Renderer* renderer, const TextElement* textElement) {
    const Font* font = &renderer->font;
    const float scale = textElement->textScale;

    float width = 0.0f;
    float maxHeight = 0.0f;

    float x = 0.0f;
    float y = 0.0f;

    for (int i = 0; i < textElement->text.length; i++) {
        const char c = textElement->text.content[i];
        if (c < 32 || c > 126) continue;

        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->glyphs,
                            font->fontAtlas.width,
                            font->fontAtlas.height,
                            c - 32,
                            &x,
                            &y,
                            &q,
                            1);

        float gw = (q.x1 - q.x0) * scale;
        float gh = (q.y1 - q.y0) * scale;

        width += gw;
        if (gh > maxHeight)
            maxHeight = gh;
    }

    return (Vec2f){ width, maxHeight };
}


void resizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);

    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->screenWidth = width;
    renderer->screenHeight = height;
}

void cursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos) {
    Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);
    renderer->mousePos.x = (float)xPos;
    renderer->mousePos.y = (float)yPos;
}

inline bool isMousePressed(GLFWwindow* window, const int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

Renderer newRenderer(const int width, const int height, const char* name, List_Element* elements) {
    return (Renderer){
        .elements = elements,
        .guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag"),
        .otherShaders = (OtherShaders){0, {}},
        .computeShader = 0,
        .window = initWindow(width, height, name),
        .render = Renderer_render,
        .screenWidth = width,
        .screenHeight = height,
        .font = loadFontAtlas("From Cartoon Blocks.ttf"),
        .basicQuadMesh = Mesh_loadSimpleQuad(),
        .defaultClick = NULL,
        .guiRoot = createRootElement()
    };
}

Renderer* newRenderer_h(const int width, const int height, const char* name, List_Element* elements) {
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->elements = elements;
    renderer->guiShader = newShader("GuiVertexShader.vert", "GuiFragmentShader.frag");
    renderer->window = initWindow(width, height, name);
    renderer->render = Renderer_render;
    renderer->screenWidth = width;
    renderer->screenHeight = height;
    renderer->font = loadFontAtlas("ARIAL.TTF");
    renderer->basicQuadMesh = Mesh_loadSimpleQuad();
    renderer->defaultClick = NULL;
    return renderer;
}

Element *guiAddElement(
    List_Element *list,
    const Mesh mesh,
    const short meshCount,
    const Vec2f pos,
    const int width,
    const int height,
    Texture *tex,
    const Vec3f color,
    const Padding padding,
    bool (*mouseOver)(const Element *, Vec2f),
    bool (*hover)(Element *, Renderer *),
    bool (*click)(Element *, Renderer *),
    const Task task,
    const char *text,
    const bool forceResize
)
{
    Element_ListAdd(list, newElement(mesh, meshCount, pos, width, height, tex));
    Element* lastElement = Element_ListGetLast(list);
    if (mouseOver) {
        lastElement->isMouseOver = mouseOver;
        if (hover) lastElement->onHover = hover;
        if (click) {
            lastElement->onClick = click;
            if (task.func) {
                lastElement->task = task;
                if (task.userdata == NULL) lastElement->task.userdata = lastElement;
            }
        }
    }
    lastElement->color = color;
    lastElement->padding = padding;

    if (text) {

        lastElement->textElement = (TextElement) {
            .offset = (Vec2f){2, 2},
            .text = newReservedString(512),
            .textScale = 1.0f,
            .textColor = (Vec3f){1.0f, 1.0f, 1.0f},
            .forceResize = forceResize,
        };
        setText(lastElement, text);
        lastElement->hasText = true;
    }
    return lastElement;
}

Element *guiAddSimpleRectangle(
    List_Element *list,
    const Vec2f pos,
    const int width,
    const int height,
    Texture *tex
)
{
    Element* element = guiAddElement(list, quadMesh, 1, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10},NULL, NULL, NULL, (Task){NULL, NULL}, NULL, false);
    return element;
}

Element *guiAddSimpleButton_Texture(
    List_Element *list,
    const Vec2f pos,
    const int width,
    const int height,
    Texture *tex,
    const Task task,
    const char *text
)
{
    Element* element = guiAddElement(list, quadMesh, 1, pos, width, height, tex, (Vec3f){0.6f, 0.6f, 0.6f}, (Padding){10, 10, 10, 10}, isSelected_Quad, hoverCallbackFunction, clickCallbackFunction, task, text, true);
    return element;
}

Element *guiAddSimpleButton_Color(
    List_Element *list,
    const Vec2f pos,
    const int width,
    const int height,
    const Vec3f color,
    const Task task,
    const char *text
)
{
    Element* element = guiAddElement(list, quadMesh, 1, pos, width, height, NULL, color, (Padding){10, 10, 10, 10}, isSelected_Quad, hoverCallbackFunction, clickCallbackFunction, task, text, true);
    return element;
}

Element createRootElement() {
    return newElement((Mesh){}, 0, (Vec2f){}, 0, 0, NULL);
}

void loadDefaultQuadMesh() {
    quadMesh = Mesh_loadSimpleQuad();
}


void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}

