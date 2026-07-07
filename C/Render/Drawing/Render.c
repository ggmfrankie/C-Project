//
// Created by Stefan on 10.10.2025.
//
#include "../../../Dependencies/include/glad/gl.h"
#include "Render.h"

#include "Batcher.h"
#include "RenderTypes.h"
#include "../../Utils/Vector.h"
#include "GLFW/glfw3.h"
#include "Render/GUI/GuiElement.h"
#include "Utils/CArrayList.h"
#include "Utils/Makros.h"
#include "Utils/UtilityFun.h"

static void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *vt, int *indices, int *id);
static Vec2i updateLayout(Element* self, Vec2i parentCursor, Vec2i remainingSpace, Vec2i parentPos, const Font* font);

static void layoutElement(const Element* self);
static Cache* cacheLayoutLines(Element* self);
static Cache* cacheLayout(Element* self);

Element* createRootElement();

[[deprecated]]
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

void Renderer_init(Renderer *renderer) {
    //ComputeShader_createUniform(&renderer->computeShader, ("dataSize"));
    //ComputeShader_createUniform(&renderer->computeShader, ("thickness"));
    //ComputeShader_update(&renderer->computeShader, graphingFunction);

    Batcher_init();
    Shader_createUniform(&renderer->guiShader, "screenWidth");
    Shader_createUniform(&renderer->guiShader, "screenHeight");
}


static void beginScissor(const Element* e, const int screenHeight) {
    glEnable(GL_SCISSOR_TEST);

    const int x = e->dims.worldPos.x;
    const int y = screenHeight - (e->dims.worldPos.y + e->dims.worldHeight);
    const int w = e->dims.worldWidth;
    const int h = e->dims.worldHeight;

    glScissor(x, y, w, h);
}

static void endScissor() {
    glDisable(GL_SCISSOR_TEST);
}

void Renderer_render(const Renderer *renderer) {
    static GuiVertex vertices[MAX_GUI_VERTICES];
    int numVertices = 0;
    static int indices[MAX_GUI_VERTICES];
    int numIndices = 0;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


#if GUI_DEBUG && GUI_DEBUG_RENDER
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

    for_eachArr(element, guiRoot->aChildElements, {
        accumulateMeshes(*element,
                    renderer,
                    vertices,
                    &numVertices,
                    indices,
                    &numIndices
        );
    });

    uploadBatchedQuads(vertices, numVertices, indices, numIndices);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDisable(GL_MULTISAMPLE);
    Shaders.unbind();
}

static void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *vt, int *indices, int *id) {
    if (element == nullptr || !element->flags.isActive) return;

    beginScissor(element, renderer->screenHeight);

    if (!element->flags.invisible) {
        element->generateMesh(element, vertices, vt, indices, id);
    }
    uploadElementData(element);
    accumulateTextQuads(element, vertices, vt, indices, id, &renderer->font);

    for_eachArr(childElement, element->aChildElements, {
        accumulateMeshes(*childElement, renderer, vertices, vt, indices, id);
    });

    endScissor();
}

void Renderer_updateLayout2(const Renderer *renderer) {
    Element* root = renderer->guiRoot;

    root->dims.worldWidth = renderer->screenWidth;
    root->dims.worldHeight = renderer->screenHeight;

    cacheLayout(root);

    layoutElement(renderer->guiRoot);
}

static void clearCache(Element* self) {
    Cache* cache = &self->layoutCache;
    arrClear(cache->aLines);
    arrPush(cache->aLines, (Line){});
    cache->minWidth = 0;
    cache->minHeight = 0;
}

static Vec2i calculateTextSize(const Element* self) {
    if (self->textElement.hasText) {
        return measureElementText(&self->textElement);
    }
    return (Vec2i){0, 0};
}

static Vec2i getManualDims(const Element* self) {
    return (Vec2i){
        self->dims.width,
        self->dims.height
    };
}
static Line* startNewline(Line* lines, const int i) {
    //Close line
    arrGetLast(lines)->end = i;
    //New line
    arrPush(lines, (Line){.start = i, .end = i});
    return arrGetLast(lines);
}

static Vec2i getDimsFromChildren(const Element* self, Line* lines) {
    Vec2i cursor = {0, 0};
    Vec2i extend = {0, 0};

    for_eachArr(childPtr, self->aChildElements, {
        Element* child = *childPtr;
        //First calculate sizes of children
        const Cache* childCache = cacheLayout(child);
        Line* currLine = &lines[0];
        const int childGap = self->childGap;

        if (child->positionMode == POS_RELATIVE) {
            extend.x = max(extend.x, child->dims.pos.x + childCache->minWidth + childGap);
            extend.y = max(extend.y, child->dims.pos.y + child->dims.height + childGap);
            currLine->end++;
            continue;
        }

        switch (self->layoutDirection) {
            case L_down: {
                const int extraGap = (cursor.y == 0) ? childGap : 0;
                const int totalPadding = self->padding.left + self->padding.right;
                const int predictedTotalHeight = cursor.y + childCache->minHeight + extraGap + totalPadding;
                if (predictedTotalHeight > self->dims.maxHeight) {
                    cursor.y = 0;
                    cursor.x = extend.x + childGap;
                    currLine = startNewline(lines, i);
                }
                //Increment cursor by childHeight + childGap
                cursor.y += childCache->minHeight + childGap;

                //Update max extend
                extend.y = max(extend.y, cursor.y);
                extend.x = max(extend.x, cursor.x + childCache->minWidth);
            }
                break;
            case L_right: {
                const int extraGap = (cursor.x == 0) ? childGap : 0;
                const int totalPadding = self->padding.up + self->padding.down;
                const int predictedTotalHeight = cursor.x + childCache->minWidth + extraGap + totalPadding;
                if (predictedTotalHeight > self->dims.maxWidth) {
                    cursor.x = 0;
                    cursor.y = extend.y + childGap;
                    currLine = startNewline(lines, i);
                }
                //Increment cursor by childWidth + childGap
                cursor.x += childCache->minWidth + childGap;

                //Update max extend
                extend.x = max(extend.x, cursor.x);
                extend.y = max(extend.y, cursor.y + childGap + childCache->minHeight);
            }
                break;
        }
        currLine->end++;
    });

    //remove extra child gap
    if (!arrIsEmpty(self->aChildElements)) {
        switch (self->layoutDirection) {
            case L_down:
                extend.y -= self->childGap;
                break;
            case L_right:
                extend.x -= self->childGap;
                break;
        }
    }

    return extend;
}

static Cache* cacheLayout(Element* self) {
    if (!self->flags.isActive) return &self->layoutCache;

    clearCache(self);

    const Vec2i textDims   = calculateTextSize(self);
    const Vec2i manualDims = getManualDims(self);

    //Recursion
    const Vec2i childDims  = getDimsFromChildren(self, self->layoutCache.aLines);

    self->layoutCache.minWidth  =
        max(self->dims.cornerRadius, self->padding.left) +
        max(textDims.x, max(manualDims.x, childDims.x)) +
        max(self->dims.cornerRadius, self->padding.right);

    self->layoutCache.minHeight =
        max(self->dims.cornerRadius, self->padding.up) +
        max(textDims.y, max(manualDims.y, childDims.y)) +
        max(self->dims.cornerRadius, self->padding.down);

    return &self->layoutCache;
}

struct FlexData {
    float totalFlex;
    float totalMinSize;
};

static struct FlexData calculateTotalLineFlex(const Line* line, const Element** elements, LayoutDirection dir) {
    struct FlexData data = {};
    for (int i = line->start; i < line->end; ++i) {
        data.totalFlex += elements[i]->dims.flexGrow;
        switch (dir) {
            case L_right: data.totalMinSize += elements[i]->layoutCache.minWidth;  break;
            case L_down:  data.totalMinSize += elements[i]->layoutCache.minHeight; break;
        }
    }
    return data;
}

static void placeChildElements(const Element* self) {
    if (!self || !self->flags.isActive) return;

    const Vec2i start = {self->padding.left, self->padding.up};
    Vec2i cursor = start;
    Vec2i extend = start;

    for_eachArr(linesPtr, self->layoutCache.aLines, {
        Line* curr = linesPtr;

        struct FlexData flexData = calculateTotalLineFlex(curr, self->aChildElements, self->layoutDirection);
    });
}

static void placeLine(const Element* parent, Vec2i* cursor, Vec2i* extend, Element** array, Line line) {
    float totalLineFlex = 0.0f;
    int totalMinSize = 0;
    int sumChildGaps = (line.end - line.start-1) * parent->childGap;

    for (int i = line.start; i < line.end; ++i) {
        totalLineFlex += array[i]->dims.flexGrow;
        switch (parent->layoutDirection) {
            case L_right: totalMinSize += array[i]->layoutCache.minWidth;  break;
            case L_down:  totalMinSize += array[i]->layoutCache.minHeight; break;
        }
    }
    int containerSize;
    switch (parent->layoutDirection) {
        case L_right: containerSize = parent->dims.worldWidth  - parent->padding.left - parent->padding.right; break;
        case L_down:  containerSize = parent->dims.worldHeight - parent->padding.up   - parent->padding.down;  break;
    }
    int availableFlexSpace = max(0, containerSize - totalMinSize - sumChildGaps);


    const Vec2i lineStart = *cursor;

    for (int i = line.start; i < line.end; ++i) {
        Element* curr = array[i];
        if (curr->positionMode == POS_FIT) {
            curr->dims.worldPos = *cursor;

            switch (parent->layoutDirection) {
                case L_down: {
                    int extraHeight = (totalLineFlex > 0) ?
                        (int)((curr->dims.flexGrow / totalLineFlex) * availableFlexSpace) : 0;
                    curr->dims.worldWidth  = curr->layoutCache.minWidth;
                    curr->dims.worldHeight = curr->layoutCache.minHeight + extraHeight;
                    cursor->y += curr->dims.worldHeight + parent->childGap;
                    break;
                }
                case L_right: {
                    int extraWidth = (totalLineFlex > 0) ?
                        (int)((curr->dims.flexGrow / totalLineFlex) * availableFlexSpace) : 0;
                    curr->dims.worldWidth  = curr->layoutCache.minWidth + extraWidth;
                    curr->dims.worldHeight = curr->layoutCache.minHeight;
                    cursor->x += curr->dims.worldWidth + parent->childGap;
                    break;
                }
            }

            extend->x = max(extend->x, curr->dims.worldPos.x + curr->dims.worldWidth);
            extend->y = max(extend->y, curr->dims.worldPos.y + curr->dims.worldHeight);

        } else {
            curr->dims.worldWidth = curr->layoutCache.minWidth;
            curr->dims.worldHeight = curr->layoutCache.minHeight;

            curr->dims.worldPos = (Vec2i){
                parent->dims.worldPos.x + curr->dims.pos.x + parent->padding.left,
                parent->dims.worldPos.y + curr->dims.pos.y + parent->padding.up
            };
        }

#if GUI_DEBUG
        const bool correctElement = (curr->name) ? (strcmp(GUI_DEBUG_OBSERVE_ELEMENT_UPDATE_ELEMENT, curr->name) == 0) : false;
        const bool print = correctElement && only_every(200);

        print_if(print,
            "---------------------\n"
            "Inside placeLine()"
            "Current Element: \"%s\"\n"
            "Dimensions:\n"
            "X: %i, Y: %i\n"
            "---------------------\n\n",
            curr->name, curr->dims.worldWidth, curr->dims.worldHeight
        );
#endif
    }
}

/// Parent element self is already sized and positioned. Now all child elements get positioned and resized if necessary

static void layoutElement(const Element* self) {
    if (!self || !self->flags.isActive) return;
    const Vec2i contentStart = {self->dims.worldPos.x + self->padding.left, self->dims.worldPos.y + self->padding.up};
    Vec2i cursor = contentStart;
    Vec2i extend = contentStart;

#if GUI_DEBUG
    const bool correctElement = (self->name) ? (strcmp(GUI_DEBUG_OBSERVE_ELEMENT_UPDATE_ELEMENT, self->name) == 0) : false;
    const bool print = correctElement && only_every(200);

    print_if(print,
        "---------------------\n"
        "Current Element: \"%s\"\n"
        "Dimensions:\n"
        "X: %i, Y: %i\n"
        "---------------------\n\n",
        self->name, self->dims.worldWidth, self->dims.worldHeight
    );
#endif

    if (!arrIsEmpty(self->aChildElements) && !arrIsEmpty(self->layoutCache.aLines)) {
        for_eachArr(linesPtr, self->layoutCache.aLines, {
            switch (self->layoutDirection) {
                case L_down:
                    cursor.x = extend.x + ((i == 0) ? 0 : self->childGap);
                    cursor.y = contentStart.y;
                    break;
                case L_right:
                    cursor.y = extend.y + ((i == 0) ? 0 : self->childGap);
                    cursor.x = contentStart.x;
                    break;
            }
            placeLine(self, &cursor, &extend, self->aChildElements, *linesPtr);
        });
    }

    for_eachArr(childPtr, self->aChildElements, {
        Element* child = *childPtr;
        if (child->callbacks.reset) child->callbacks.reset(child);
        layoutElement(*childPtr);
    });
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
    const auto e = Element_new((Vec2i){}, 0, 0);
    e->name = "GUI_ROOT";
    return e;
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}