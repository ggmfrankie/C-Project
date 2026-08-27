#include "Scrollbar.h"

#include <stdlib.h>

#include "Makros/Makros.h"

static void Scrollbar_handleDragging(Element *element, Vec2f pos) {
    const float newY = max(0, min(Element_get(element->parentElement)->dims.worldHeight - element->dims.worldHeight, pos.y));
    ScrollbarData* data = element->elementData.ptr;

    data->absPos = newY;
    data->normPos = newY / (Element_get(element->parentElement)->dims.worldHeight - element->dims.worldHeight);

    if (data->onMove) data->onMove(data->normPos, data->absPos);

    element->dims.pos.y = newY;
}

ElementHandle Scrollbar_new(ScrollbarSettings settings) {
    const ElementHandle sliderHandle = Element_new((ElementSettings) {
        .pos = {0, 0},
        .minWidth = settings.railWidth,
        .minHeight = settings.sliderHeight,
        .posMode = POS_RELATIVE,
        .color = {0,0,0},
        .transparency = 0.5,
        .cornerRadius = settings.railWidth*0.5
    });

    const ElementHandle railHandle = Element_new((ElementSettings) {
        .pos = settings.pos,
        .minWidth = settings.railWidth,
        .wantGrowVertical = true,
        .color = {0.3,.3,.3},
        .cornerRadius = settings.railWidth*0.5
    },
        sliderHandle
    );

    Element* slider = Element_get(sliderHandle);
    ScrollbarData* data = calloc(1, sizeof(ScrollbarData));
    data->onMove = settings.onMove;

    slider->elementData.ptr = data;
    slider->elementData.needsFree = true;

    slider->callbacks.requestMove = Scrollbar_handleDragging;

    return railHandle;
}


