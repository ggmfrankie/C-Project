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

static void placeChildElements(const Element* self);

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

    Element* guiRoot = renderer->guiRoot;

    accumulateMeshes(guiRoot,
            renderer,
            vertices,
            &numVertices,
            indices,
            &numIndices
    );

    uploadBatchedQuads(vertices, numVertices, indices, numIndices);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDisable(GL_MULTISAMPLE);
    Shaders.unbind();
}

static void accumulateMeshes(Element *element, const Renderer *renderer, GuiVertex *vertices, int *vt, int *indices, int *id) {
    if (element == nullptr || !element->flags.isActive) return;

    //beginScissor(element, renderer->screenHeight);

    if (!element->flags.invisible) {
#if GUI_DEBUG && GUI_DEBUG_ACCUMULATE_MESHES
        if (element->dims.worldWidth <= 0 || element->dims.worldHeight <= 0) {
            printf("WARNING: Element '%s' has invalid dimensions: %dx%d\n",
                   element->name ? element->name : "unnamed",
                   element->dims.worldWidth, element->dims.worldHeight);
        }
#endif
        element->generateMesh(element, vertices, vt, indices, id);
    }
    uploadElementData(element);
    accumulateTextQuads(element, vertices, vt, indices, id, &renderer->font);

    //endScissor();

    for_eachArr(flowElement, element->aFlowElements, {
        accumulateMeshes(*flowElement, renderer, vertices, vt, indices, id);
    });

    for_eachArr(staticElement, element->aStaticElements, {
        accumulateMeshes(*staticElement, renderer, vertices, vt, indices, id);
    });
}

void Renderer_updateLayout2(const Renderer *renderer) {
    Element* root = renderer->guiRoot;

    root->dims.worldWidth = renderer->screenWidth;
    root->dims.worldHeight = renderer->screenHeight;

    cacheLayout(root);

    placeChildElements(renderer->guiRoot);
    //layoutElement(renderer->guiRoot);
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

static Line* createNewLine(Line* lines, const int i) {
    arrGetLast(lines)->end = i;
    arrPush(lines, (Line){.start = i, .end = i});
    return arrGetLast(lines);
}

static Vec2i getDimsFromFlowChildren(const Element* self) {
    Vec2i cursor = {0, 0};
    Vec2i extend = {0, 0};
    Line* lines = self->layoutCache.aLines;
    Line* currLine = &lines[0];

    for_eachArr(childPtr, self->aFlowElements, {
        Element* child = *childPtr;
        //First calculate sizes of children
        const Cache* childCache = cacheLayout(child);
        const int childGap = self->childGap;

        switch (self->layoutDirection) {
            case LAYOUT_DOWN: {
                const int extraGap = (cursor.y == 0) ? 0 : childGap;
                const int totalPadding = self->padding.up + self->padding.down;
                const int predictedTotalHeight = cursor.y + childCache->minHeight + extraGap + totalPadding;

                if (predictedTotalHeight > self->dims.maxHeight) {
                    cursor.y = 0;
                    cursor.x = extend.x + childGap;

                    currLine = createNewLine(lines, i);
                }
                //Increment cursor by childHeight + childGap
                cursor.y += childCache->minHeight + childGap;

                //Update max extend
                extend.y = max(extend.y, cursor.y);
                extend.x = max(extend.x, cursor.x + childCache->minWidth);
            }
                break;
            case LAYOUT_RIGHT: {
                const int extraGap = (cursor.x == 0) ? 0 : childGap;
                const int totalPadding = self->padding.left + self->padding.right;
                const int predictedTotalWidth = cursor.x + childCache->minWidth + extraGap + totalPadding;

                if (predictedTotalWidth > self->dims.maxWidth) {
                    cursor.x = 0;
                    cursor.y = extend.y + childGap;

                    currLine = createNewLine(lines, i);
                }
                //Increment cursor by childWidth + childGap
                cursor.x += childCache->minWidth + childGap;

                //Update max extend
                extend.x = max(extend.x, cursor.x);
                extend.y = max(extend.y, cursor.y + childCache->minHeight);
            }
                break;
        }
        currLine->end++;
    });

    //remove extra child gap
    if (!arrIsEmpty(self->aFlowElements)) {
        switch (self->layoutDirection) {
            case LAYOUT_DOWN:
                extend.y -= self->childGap;
                break;
            case LAYOUT_RIGHT:
                extend.x -= self->childGap;
                break;
        }
    }

    return extend;
}

static Vec2i getDimsFromStaticChildren(const Element* self) {
    Vec2i extend = {0, 0};
    for_eachArr(childPtr, self->aStaticElements, {
        Element* child = *childPtr;
        const Vec2i pos = child->dims.pos;
        //Children first
        const Cache* childCache = cacheLayout(child);
        extend.x = max(extend.x, pos.x + childCache->minWidth);
        extend.y = max(extend.y, pos.y + childCache->minHeight);
    });
    return extend;
}

static Vec2i getDimsFromChildren(const Element* self) {
    const Vec2i flowDims   = getDimsFromFlowChildren(self);
    const Vec2i staticDims = getDimsFromStaticChildren(self);

    return (Vec2i){
        max(flowDims.x, staticDims.x),
        max(flowDims.y, staticDims.y)
    };
}

static Cache* cacheLayout(Element* self) {
    if (!self->flags.isActive) return &self->layoutCache;

    clearCache(self);

    const Vec2i textDims   = calculateTextSize(self);
    const Vec2i manualDims = getManualDims(self);

    //Recursion
    const Vec2i childDims  = getDimsFromChildren(self);

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
    Vec2i totalMinSize;
};

static struct FlexData calculateTotalLineFlex(const Line* line, Element** elements) {
    struct FlexData data = {};
    for (int i = line->start; i < line->end; ++i) {
        data.totalFlex += elements[i]->dims.flexGrow;
        data.totalMinSize.x += elements[i]->layoutCache.minWidth;
        data.totalMinSize.y += elements[i]->layoutCache.minHeight;
    }
    return data;
}

static void placeElementAt(Element* self, Vec2i pos, Vec2i dims) {
    self->dims.worldPos    = pos;
    self->dims.worldWidth  = dims.x;
    self->dims.worldHeight = dims.y;
}

static void placeFlowElements(const Element* self) {
    const Vec2i start = {self->dims.worldPos.x + self->padding.left, self->dims.worldPos.y + self->padding.up};
    Vec2i cursor = start;
    Vec2i extend = start;

    for_eachArr(linesPtr, self->layoutCache.aLines, {
        Line currLine = *linesPtr;
        if (currLine.start == currLine.end) continue;

        const struct FlexData flexData = calculateTotalLineFlex(&currLine, self->aFlowElements);

        for (int j = currLine.start; j < currLine.end; ++j) {
            Element* curr = self->aFlowElements[j];

            Vec2i dims = {
                curr->layoutCache.minWidth,
                curr->layoutCache.minHeight
            };

            const int totalChildGap = ((currLine.end - currLine.start)-1) * self->childGap;
            const Vec2i totalPadding = {self->padding.left + self->padding.right, self->padding.up + self->padding.down};
            const float flexFactor  = flexData.totalFlex ? (curr->dims.flexGrow/flexData.totalFlex) : 0.0f;

            switch (self->layoutDirection) {
                case LAYOUT_RIGHT:
                    //TODO: fix -> make any line growable
                    //Flex space
                    dims.x += flexFactor * (self->dims.worldWidth  - flexData.totalMinSize.x - totalChildGap - totalPadding.x);
                    //Want to grow
                    if (i == len-1 && curr->flags.wantGrowVertical) {
                        const int parentSpace = (self->dims.worldHeight - totalPadding.y);
                        const int freeSpace = parentSpace - dims.y;
                        dims.y += freeSpace;
                    }
                    break;
                case LAYOUT_DOWN:
                    if (i == len-1 && curr->flags.wantGrowHorizontal) {
                        const int parentSpace = (self->dims.worldWidth - totalPadding.x);
                        const int freeSpace = parentSpace - dims.x;
                        dims.x += freeSpace;
                    }
                    dims.y += flexFactor * (self->dims.worldHeight - flexData.totalMinSize.y - totalChildGap - totalPadding.y);
                    break;
            }

            placeElementAt(curr, cursor, dims);

            extend.x = max(extend.x, cursor.x + dims.x);
            extend.y = max(extend.y, cursor.y + dims.y);

            switch (self->layoutDirection) {
                case LAYOUT_RIGHT: cursor.x += self->childGap + dims.x; break;
                case LAYOUT_DOWN:  cursor.y += self->childGap + dims.y; break;
            }
        }
        switch (self->layoutDirection) {
            case LAYOUT_RIGHT:
                cursor.x = start.x;
                cursor.y = extend.y + self->childGap;
                break;
            case LAYOUT_DOWN:
                cursor.x = extend.x + self->childGap;
                cursor.y = start.y;
                break;
        }
    });
}

static void placeStaticElements(const Element* self) {
    for_eachArr(elementPtr, self->aStaticElements, {
        Element* curr = * elementPtr;
        placeElementAt(curr,
            (Vec2i){
                self->dims.worldPos.x + curr->dims.pos.x + self->padding.up,
                self->dims.worldPos.y + curr->dims.pos.y + self->padding.left
            },
            (Vec2i){
                curr->layoutCache.minWidth,
                curr->layoutCache.minHeight
            }
        );
    });
}

static void placeChildElements(const Element* self) {
    if (!self || !self->flags.isActive) return;
    placeFlowElements(self);
    placeStaticElements(self);

    for_eachArr(flowElement, self->aFlowElements, { placeChildElements(*flowElement); });
    for_eachArr(staticElement, self->aStaticElements,{ placeChildElements(*staticElement); });

#if GUI_DEBUG
    const bool correctElement = (self->name) ? (strcmp(GUI_DEBUG_OBSERVE_ELEMENT_PLACE_CHILDREN, self->name) == 0) : false;
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
    e->flags.invisible = true;
    return e;
}

void Renderer_destroy(const Renderer *renderer) {
    glfwDestroyWindow(renderer->window);
}