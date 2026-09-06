//
// Created by Stefan on 10.10.2025.
//
#include "Layout.h"

#include "glad/gl.h"

#include <string.h>

#include "../Drawing/RenderTypes.h"
#include "Math/Vector.h"
#include "GUI/GuiElement/GuiElement.h"
#include "DataStructures/CArrayList.h"
#include "GUI/Engine.h"
#include "Makros/Makros.h"
#include "Utils/Misc/UtilityFun.h"

static Cache* cacheLayout(Element* self);

static void placeChildElements(const Element* self);

static void updateGuiRoot(const GuiState *renderer, Element* root) {
    root->dims.worldWidth  = renderer->screenWidth;
    root->dims.worldHeight = renderer->screenHeight;

    root->dims.maxWidth  = renderer->screenWidth;
    root->dims.maxHeight = renderer->screenHeight;

    root->visuals.clip.dims = (Vec2f){renderer->screenWidth, renderer->screenHeight};
}

void Layout_updateLayout(const GuiState *state) {
    Element* root = Element_get(state->guiRoot);

    updateGuiRoot(state, root);

    cacheLayout(root);

    placeChildElements(root);
}

static void clearCache(Element* self) {
    Cache* cache = &self->layoutCache;
    arrClear(cache->aLines);
    arrPush(cache->aLines, (Line){});
    cache->minWidth = 0;
    cache->minHeight = 0;
}

static Vec2f calculateTextSize(const Element* self) {
    if (self->textElement.hasText) {
        return Text_measureElementText(&self->textElement);
    }
    return (Vec2f){0, 0};
}

static Vec2f getManualDims(const Element* self) {
    return (Vec2f){
        self->dims.width,
        self->dims.height
    };
}

static Line* createNewLine(Line* lines, const int i) {
    arrGetLast(lines)->end = i;
    arrPush(lines, (Line){.start = i, .end = i});
    return arrGetLast(lines);
}

static Vec2f getDimsFromFlowChildren(const Element* self) {
    Vec2f cursor = {0, 0};
    Vec2f extend = {0, 0};
    Line* lines = self->layoutCache.aLines;
    Line* currLine = &lines[0];

    for_eachArr(childPtr, self->aFlowElements, {
        Element* child = Element_get(*childPtr);
        //First calculate sizes of children
        const Cache* childCache = cacheLayout(child);
        const float childGap = self->childGap;

        switch (self->layoutDirection) {
            case LAYOUT_DOWN: {
                const float extraGap = (cursor.y == 0) ? 0 : childGap;
                const float totalPadding = self->padding.up + self->padding.down;
                const float predictedTotalHeight = cursor.y + childCache->minHeight + extraGap + totalPadding;

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
                const float extraGap = (cursor.x == 0) ? 0 : childGap;
                const float totalPadding = self->padding.left + self->padding.right;
                const float predictedTotalWidth = cursor.x + childCache->minWidth + extraGap + totalPadding;

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

static Vec2f getDimsFromStaticChildren(const Element* self) {
    Vec2f extend = {0, 0};
    for_eachArr(childPtr, self->aStaticElements, {
        Element* child = Element_get(*childPtr);
        const Vec2f pos = child->dims.pos;
        //Children first
        const Cache* childCache = cacheLayout(child);
        extend.x = max(extend.x, pos.x + childCache->minWidth);
        extend.y = max(extend.y, pos.y + childCache->minHeight);
    });
    return extend;
}

static Vec2f getDimsFromChildren(const Element* self) {
    const Vec2f flowDims   = getDimsFromFlowChildren(self);
    const Vec2f staticDims = getDimsFromStaticChildren(self);

    return (Vec2f){
        max(flowDims.x, staticDims.x),
        max(flowDims.y, staticDims.y)
    };
}

static Cache* cacheLayout(Element* self) {
    if (!self->flags.isActive) return &self->layoutCache;

    clearCache(self);

    const Vec2f textDims   = calculateTextSize(self);
    const Vec2f manualDims = getManualDims(self);

    //Recursion
    const Vec2f childDims  = getDimsFromChildren(self);

    self->layoutCache.minWidth  =
        self->padding.left +
        max(textDims.x, max(manualDims.x, childDims.x)) +
        self->padding.right;

    self->layoutCache.minHeight =
        self->padding.up +
        max(textDims.y, max(manualDims.y, childDims.y)) +
        self->padding.down;

    return &self->layoutCache;
}

struct FlexData {
    float totalFlex;
    Vec2f totalMinSize;
};

static struct FlexData calculateTotalLineFlex(const Line* line, const ElementHandle* elements) {
    struct FlexData data = {};
    for (int i = line->start; i < line->end; ++i) {
        const Element* curr = Element_get(elements[i]);
        data.totalFlex += curr->dims.flexGrow;
        data.totalMinSize.x += curr->layoutCache.minWidth;
        data.totalMinSize.y += curr->layoutCache.minHeight;
    }
    return data;
}

static void placeElementAt(Element* self, Vec2f pos, Vec2f dims) {
    self->dims.worldPos    = pos;
    self->dims.worldWidth  = dims.x;
    self->dims.worldHeight = dims.y;
}

static void placeFlowElements(const Element* self) {
    const Vec2f start = {self->dims.worldPos.x + self->padding.left, self->dims.worldPos.y + self->padding.up};
    Vec2f cursor = start;
    Vec2f extend = start;

    for_eachArr(linesPtr, self->layoutCache.aLines, {
        Line currLine = *linesPtr;
        if (currLine.start == currLine.end) continue;

        const struct FlexData flexData = calculateTotalLineFlex(&currLine, self->aFlowElements);

        for (int j = currLine.start; j < currLine.end; ++j) {
            Element* curr = Element_get(self->aFlowElements[j]);

            Vec2f dims = {
                curr->layoutCache.minWidth,
                curr->layoutCache.minHeight
            };

            const float totalChildGap = ((currLine.end - currLine.start)-1) * self->childGap;
            const Vec2f totalPadding = {self->padding.left + self->padding.right, self->padding.up + self->padding.down};
            const float flexFactor  = flexData.totalFlex ? (curr->dims.flexGrow/flexData.totalFlex) : 0.0f;

            switch (self->layoutDirection) {
                case LAYOUT_RIGHT:
                    //TODO: fix -> make any line growable
                    //Flex space
                    dims.x += flexFactor * (self->dims.worldWidth  - flexData.totalMinSize.x - totalChildGap - totalPadding.x);
                    //Want to grow
                    if (i == len-1 && curr->flags.wantGrowVertical) {
                        const float freeSpace = (self->dims.worldHeight - totalPadding.y);
                        dims.y = freeSpace;
                    }
                    break;
                case LAYOUT_DOWN:
                    if (i == len-1 && curr->flags.wantGrowHorizontal) {
                        const float freeSpace = (self->dims.worldWidth - totalPadding.x);
                        dims.x = freeSpace;
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
        Element* curr = Element_get(*elementPtr);
        placeElementAt(curr,
            (Vec2f){
            self->dims.worldPos.x + curr->dims.pos.x + self->padding.up,
            self->dims.worldPos.y + curr->dims.pos.y + self->padding.left
            },
            (Vec2f){
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

    for_eachArr(flowElement, self->aFlowElements, { placeChildElements(Element_get(*flowElement)); });
    for_eachArr(staticElement, self->aStaticElements,{ placeChildElements(Element_get(*staticElement)); });
#if GUI_DEBUG
    const bool correctElement = (self->name) ? (strcmp(GUI_DEBUG_OBSERVE_ELEMENT_PLACE_CHILDREN, self->name) == 0) : false;
    const bool print = correctElement && only_every(200);

    print_if(print,
        "---------------------\n"
        "Current Element: \"%s\"\n"
        "Dimensions:\n"
        "X: %f, Y: %f\n"
        "---------------------\n\n",
        self->name, self->dims.worldWidth, self->dims.worldHeight
    );
#endif
}


