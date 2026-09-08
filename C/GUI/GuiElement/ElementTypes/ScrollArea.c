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

static void ScrollArea_updateScrollbar(const Element* scrollArea) {
    assert(scrollArea->type == ELEMENT_TYPE_SCROLL_AREA);

    const ScrollAreaData* data = scrollArea->elementData.ptr;
    const Element* panel = Element_get(data->contentArea);
    const Element* scrollbar = Element_get(data->scrollbar);

    const float sliderHeight = scrollArea->dims.worldHeight * (scrollArea->dims.worldHeight / panel->dims.worldHeight);
    Scrollbar_setSliderHeight(scrollbar, sliderHeight);
}

static void ScrollArea_scrollbarCallback(const ScrollbarData* scrollbarData, Element* movedElement) {
    const float remainder = movedElement->dims.worldHeight - Element_get(scrollbarData->rail)->dims.worldHeight;
    movedElement->dims.pos.y = -(remainder * scrollbarData->progress);
}

ElementHandle _ScrollArea_new(ScrollAreaSettings settings, ...) {
    const ElementHandle contentArea = Element_new((ElementSettings){
        .pos = {},
        .wantGrowHorizontal = true,
        .invisible = true,
        .noLayoutContributionVertical = true,
        .posMode = POS_RELATIVE,
        .childGap = settings.childGap,
        .padding = settings.padding,
        .notSelectable = settings.notSelectable
    });

    const ElementHandle panel = Element_new((ElementSettings){
        .invisible = true,
        .notSelectable = true
    },
        contentArea
    );

    const ElementHandle scrollbar = Scrollbar_new((ScrollbarSettings){
        .pos = {},
        .railWidth = 10,
        .sliderHeight = 20,
        .onMove = ScrollArea_scrollbarCallback,
        .moveElement = contentArea,
        .sliderColor = settings.sliderColor,
        .railColor = settings.railColor
    });

    const ElementHandle frame = Element_new((ElementSettings){
        .pos = settings.pos,
        .minWidth = settings.width,
        .minHeight = settings.height,
        .useClipping = true,
        .color = settings.backgroundColor,
        .posMode = POS_RELATIVE,
        .layoutDirection = LAYOUT_RIGHT,
        .cornerRadius = 5,
        .notSelectable = settings.notSelectable
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