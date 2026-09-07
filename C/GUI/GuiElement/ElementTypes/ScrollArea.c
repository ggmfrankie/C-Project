//
// Created by Stefan on 06.09.2026.
//

#include "ScrollArea.h"

#include <assert.h>
#include <stdlib.h>

#include "Scrollbar.h"
typedef struct {
    ElementHandle contentArea;
    ElementHandle scrollbar;
} ScrollAreaData;

static void ScrollArea_updateScrollbar(Element* scrollArea) {
    assert(scrollArea->type == ELEMENT_TYPE_SCROLL_AREA);

    const ScrollAreaData* data = scrollArea->elementData.ptr;
    Element* panel = Element_get(data->contentArea);
    Element* scrollbar = Element_get(data->scrollbar);

    float sliderHeight = scrollArea->dims.worldHeight * (scrollArea->dims.worldHeight / panel->dims.worldHeight);
    Scrollbar_setSliderHeight(scrollbar, sliderHeight);
}

static void ScrollArea_scrollbarCallback(float norm, float abs, Element* movedElement) {
    movedElement->dims.pos.y = -abs;
}

ElementHandle _ScrollArea_new(ScrollAreaSettings settings, ...) {
    const ElementHandle contentArea = Element_new((ElementSettings){
        .pos = {},
        .wantGrowHorizontal = true,
        .invisible = true,
        .noLayoutContribution = true,
        .posMode = POS_RELATIVE,
        .childGap = settings.childGap,
        .padding = settings.padding,
    });

    const ElementHandle panel = Element_new((ElementSettings){
        .invisible = true,
        .canNotBeSelected = true
    },
        contentArea
    );

    const ElementHandle scrollbar = Scrollbar_new((ScrollbarSettings){
        .pos = {},
        .railWidth = 10,
        .sliderHeight = 20,
        .onMove = ScrollArea_scrollbarCallback,
        .moveElement = contentArea,
    });

    const ElementHandle frame = Element_new((ElementSettings){
        .pos = settings.pos,
        .minWidth = settings.width,
        .minHeight = settings.height,
        .clipArea = {.pos = {0,0}, .dims = {settings.width, settings.height}},
        .color = settings.color,
        .posMode = POS_RELATIVE,
        .layoutDirection = LAYOUT_RIGHT,
        .cornerRadius = 5,
    },
        scrollbar,
        panel
    );

    Element* scrollArea = Element_get(frame);
    scrollArea->type = ELEMENT_TYPE_SCROLL_AREA;

    ScrollAreaData* data = calloc(1, sizeof(ScrollAreaData));
    data->contentArea = contentArea;
    data->scrollbar = scrollbar;

    scrollArea->elementData.ptr = data;
    scrollArea->elementData.needsFree = true;

    va_list args;
    va_start(args, settings);

    Element_addChildElements_vaList(contentArea, args);

    va_end(args);

    return frame;
}