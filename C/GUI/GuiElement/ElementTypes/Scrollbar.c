#include "Scrollbar.h"

#include <assert.h>
#include <stdlib.h>

#include "Makros/Makros.h"

static void Scrollbar_handleDragging(Element* self, Vec2f pos) {
    assert(self->type == ELEMENT_TYPE_SCROLLBAR_SLIDER);
    const float newY = max(0, min(Element_get(self->parentElement)->dims.worldHeight - self->dims.worldHeight, pos.y));
    ScrollbarData* data = self->elementData.ptr;

    data->progress = newY / (Element_get(self->parentElement)->dims.worldHeight - self->dims.worldHeight);

    if (data->onMove) data->onMove(data, Element_get(data->moveElement));

    self->dims.pos.y = newY;
}

ElementHandle Scrollbar_new(ScrollbarSettings settings) {
    const ElementHandle sliderHandle = Element_new((ElementSettings) {
        .pos = {0, 0},
        .minWidth = settings.railWidth,
        .minHeight = settings.sliderHeight,
        .posMode = POS_RELATIVE,
        .color = settings.sliderColor,
        .transparency = 0.5,
        .cornerRadius = settings.railWidth*0.5,
        .canBeHovered = true
    });

    const ElementHandle railHandle = Element_new((ElementSettings) {
        .pos = settings.pos,
        .minWidth = settings.railWidth,
        .wantGrowVertical = true,
        .color = settings.railColor,
        .cornerRadius = settings.railWidth*0.5
    },
        sliderHandle
    );

    ScrollbarData* data = calloc(1, sizeof(ScrollbarData));
    data->onMove = settings.onMove;
    data->moveElement = settings.moveElement;
    data->slider = sliderHandle;
    data->rail = railHandle;

    Element* slider = Element_get(sliderHandle);
    slider->type = ELEMENT_TYPE_SCROLLBAR_SLIDER;
    slider->elementData.ptr = data;
    slider->elementData.needsFree = false;

    slider->callbacks.requestMove = Scrollbar_handleDragging;

    Element* rail = Element_get(railHandle);
    rail->type = ELEMENT_TYPE_SCROLLBAR_RAIL;
    rail->elementData.ptr = data;
    rail->elementData.needsFree = true;

    return railHandle;
}

void Scrollbar_updatePos(const Element* rail) {
    assert(rail->type == ELEMENT_TYPE_SCROLLBAR_RAIL);
    const ScrollbarData* data = rail->elementData.ptr;
    Element* slider = Element_get(data->slider);

    slider->dims.pos.y = (rail->dims.worldHeight - slider->dims.worldHeight) * data->progress;
}

void Scrollbar_setSliderHeight(const Element* rail, float newHeight) {
    assert(rail->type == ELEMENT_TYPE_SCROLLBAR_RAIL);
    const ScrollbarData* data = rail->elementData.ptr;
    Element* slider = Element_get(data->slider);

    slider->dims.height = max(rail->dims.worldHeight, min(newHeight, 0));

    slider->dims.pos.y = (rail->dims.worldHeight - slider->dims.worldHeight) * data->progress;
}
