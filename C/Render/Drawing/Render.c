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

[[deprecated]]
void Renderer_updateLayout(const Renderer *renderer) {
    Element* root = renderer->guiRoot;
    root->dims.width = renderer->screenWidth;
    root->dims.height = renderer->screenHeight;

    updateLayout(
        root,
        (Vec2i){0, 0},
        (Vec2i){renderer->screenWidth, renderer->screenHeight},
        (Vec2i){0, 0}, &renderer->font
    );
}

void Renderer_updateLayout2(const Renderer *renderer) {
    Element* root = renderer->guiRoot;

    root->dims.worldWidth = renderer->screenWidth;
    root->dims.worldHeight = renderer->screenHeight;

    cacheLayoutLines(root);

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
        return  measureElementText(&self->textElement);
    }
    return (Vec2i){0, 0};
}

static Vec2i getManualDims(const Element* self) {
    return (Vec2i){
        self->dims.width,
        self->dims.height
    };
}



static Vec2i getDimsFromChildren(const Element* self, Cache* parentCache) {
    const Vec2i start = {self->padding.up, self->padding.left};
    Vec2i cursor = start;
    Vec2i extend = (Vec2i){0, 0};

    for_eachArr(childPtr, self->aChildElements, {
        const Cache* childCache = &(*childPtr)->layoutCache;
        Line* lines = parentCache->aLines;

        switch (self->layoutDirection) {
            case L_down: {
                int predictedExtend = extend.y + childCache->minHeight + self->childGap;
                if (predictedExtend > self->dims.maxWidth) {
                    cursor.y = start.y;
                    cursor.x = extend.x + self->childGap;
                }
            }
                break;
            case L_right: {

            }
                break;
        }
    });
}



static Cache* cacheLayout(Element* self) {
    if (!self->flags.isActive) return &self->layoutCache;

    clearCache(self);

    Vec2i textDims   = calculateTextSize(self);
    Vec2i manualDims = getManualDims(self);
    Vec2i childDims  = getDimsFromChildren(self, &self->layoutCache);
}

static Cache* cacheLayoutLines(Element* self) {
    Cache* data = &self->layoutCache;
    arrClear(data->aLines);
    arrPush(data->aLines, (Line){});
    data->minWidth = 0;
    data->minHeight = 0;

    if (!self->flags.isActive) return data;

    data->minWidth = self->padding.left + self->padding.right;
    data->minHeight = self->padding.up + self->padding.down;

    Vec2i cursor = {self->padding.left, self->padding.up};
    Vec2i extend = {};

    if (self->textElement.hasText) {
        const Vec2i textDims = measureElementText(&self->textElement);
        data->minWidth += textDims.x;
        data->minHeight += textDims.y;
    }

    for_eachArr(childPtr, self->aChildElements, {
        Element* child = *childPtr;
        const Cache* childData = cacheLayoutLines(child);

        Line* currLine = arrGetLast(data->aLines);

        if (child->positionMode == POS_FIT) {
            switch (self->layoutDirection) {
                case L_down: {
                    const int extraGap = (cursor.y == self->padding.up) ? 0 : self->childGap;
                    if (cursor.y - self->padding.down + childData->minHeight + self->padding.down + extraGap > self->dims.maxHeight) {
                        // new line
                        currLine->end = i;
                        arrPush(data->aLines, (Line){i, i});
                        currLine = arrGetLast(data->aLines);
                        cursor = (Vec2i){extend.x + self->childGap, self->padding.up};
                    }
                    cursor.y += childData->minHeight + self->childGap;

                    extend.y = max(extend.y, cursor.y);
                    extend.x = max(extend.x, cursor.x + childData->minWidth);

                    break;
                }
                case L_right: {
                    const int extraGap = (cursor.x == self->padding.left) ? 0 : self->childGap;
                    if (cursor.x - self->padding.left + childData->minWidth + self->padding.right + extraGap > self->dims.maxWidth) {
                        // new line
                        currLine->end = i;
                        arrPush(data->aLines, (Line){i, i});
                        currLine = arrGetLast(data->aLines);
                        cursor = (Vec2i){self->padding.left, extend.y + self->childGap};
                    }
                    cursor.x += childData->minWidth + self->childGap;

                    extend.x = max(extend.x, cursor.x);
                    extend.y = max(extend.y, cursor.y + childData->minHeight - self->padding.up);

                    break;
                }
            }

        } else {

            extend.x = max(extend.x, child->dims.pos.x + child->dims.width + self->childGap);
            extend.y = max(extend.y, child->dims.pos.y + child->dims.height + self->childGap);

        }
        ++currLine->end;
    });

    const int overChildGap = (arrLen(self->aChildElements) == 0) ? 0 : self->childGap;

    data->minWidth  = max(extend.x + self->padding.left + self->padding.right - overChildGap, max(self->dims.width,  data->minWidth));
    data->minHeight = max(extend.y + self->padding.up   + self->padding.down  - overChildGap, max(self->dims.height, data->minHeight));
    return data;
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
                parent->dims.worldPos.x + curr->dims.pos.x,
                parent->dims.worldPos.y + curr->dims.pos.y
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
                    placeLine(self, &cursor, &extend, self->aChildElements, *linesPtr);
                    break;
                case L_right:
                    cursor.y = extend.y + ((i == 0) ? 0 : self->childGap);
                    cursor.x = contentStart.x;
                    placeLine(self, &cursor, &extend, self->aChildElements, *linesPtr);
                    break;
            }
        });
    }

    for_eachArr(childPtr, self->aChildElements, {
        Element* child = *childPtr;
        if (child->callbacks.reset) child->callbacks.reset(child);
        layoutElement(*childPtr);
    });
}

//TODO maybe pass available size to the child element or maybe change the layout function if the element has fixed width?
//! @brief Traverses the gui_element tree and updates positions and dimensions of children and then parents
[[deprecated]]
static Vec2i updateLayout(Element* self, const Vec2i parentCursor, const Vec2i remainingSpace, const Vec2i parentPos, const Font* font) {
    if (!self || !self->flags.isActive) return (Vec2i){0,0};
    const auto cb = &self->callbacks;
    const auto dims = &self->dims;
    const auto padding = &self->padding;
    const auto flags = &self->flags; 
    
#if GUI_DEBUG
    const bool correctElement = (self->name) ? (strcmp(GUI_DEBUG_OBSERVE_ELEMENT_UPDATE_ELEMENT, self->name) == 0) : false;
    const bool print = correctElement && only_every(200);
#endif
    if (cb->reset) cb->reset(self);
#if GUI_DEBUG
    print_if(print,
        "---------------------\n"
        "Current Element: \"%s\"\n"
        "Remaining space:\n"
        "X: %i, Y: %i\n"
        "---------------------\n",
        self->name, remainingSpace.x, remainingSpace.y
    );
#endif
    if (self->positionMode == POS_FIT) {
        dims->pos.x = parentCursor.x;
        dims->pos.y = parentCursor.y;
    }
    dims->worldPos.x = dims->pos.x + parentPos.x;
    dims->worldPos.y = dims->pos.y + parentPos.y;

    dims->worldWidth = dims->width;
    dims->worldHeight = dims->height;

    auto cursor = (Vec2i){padding->left, padding->up};
    const LayoutDirection layoutDirection = self->layoutDirection;

    int extendRight = self->padding.left;
    int extendDown = self->padding.up;

    if (self->textElement.hasText) {
        const int textW = padding->left + (int)self->textElement.width + padding->right;
        const int textH = padding->up + (int)((float)(font->maxCharHeight) * self->textElement.textScale) + padding->down;

        dims->worldWidth  = max(dims->worldWidth ,  textW);
        dims->worldHeight = max(dims->worldHeight, textH);
    }

    const Vec2i maxSpace = {
        .x = flags->fixedWidth ? dims->worldWidth : self->flags.wantGrowHorizontal ? remainingSpace.x : 0,
        .y = flags->fixedHeight ? dims->worldHeight : self->flags.wantGrowVertical ? remainingSpace.y : 0
    };

    for_eachArr(childPtr, self->aChildElements, {
        Element *child = *childPtr;
        Vec2i currSpace = {.x = maxSpace.x - cursor.x, .y = maxSpace.y - cursor.y};

        Vec2i childDimensions = updateLayout(child, cursor, currSpace, dims->worldPos, font);

        // if current element has fixed dims, but child elements in the next row
        // advance the cursor to the next column
        if (flags->fixedWidth && cursor.x + childDimensions.x + padding->right > dims->worldWidth) {
            cursor.y = extendDown + self->childGap;
            cursor.x = padding->left;
            currSpace.y = dims->height + padding->up + padding->down;
            childDimensions = updateLayout(child, cursor, currSpace, dims->worldPos, font);
        }
        // advance the cursor to the next row
        if (flags->fixedHeight && cursor.y + childDimensions.y + padding->down > dims->worldHeight) {
            cursor.y = padding->up;
            cursor.x = extendRight + self->childGap;
            currSpace.x = dims->width + padding->left + padding->right;
            childDimensions = updateLayout(child, cursor, currSpace, dims->worldPos, font);
        }

        extendRight = max(extendRight, child->dims.pos.x + childDimensions.x);
        extendDown  = max(extendDown,  child->dims.pos.y + childDimensions.y);

        if (child->positionMode == POS_FIT) {
            if (layoutDirection == L_down) cursor.y += childDimensions.y + self->childGap;
            else if (layoutDirection == L_right) cursor.x += childDimensions.x + self->childGap;
        } else {
            if (layoutDirection == L_down) cursor.y = extendDown + self->childGap;
            else if (layoutDirection == L_right) cursor.x = extendRight + self->childGap;
        }
    });

    dims->worldWidth = max(dims->worldWidth, extendRight + padding->right);
    dims->worldHeight = max(dims->worldHeight, extendDown + padding->down);

    if (self->flags.wantGrowHorizontal) self->dims.worldWidth = max(dims->worldWidth, remainingSpace.x);
    if (self->flags.wantGrowVertical) self->dims.worldHeight = max(dims->worldHeight, remainingSpace.y);

    return (Vec2i){dims->worldWidth, dims->worldHeight};
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