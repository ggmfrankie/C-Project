#include "Scrollbar.h"

ElementHandle Scrollbar_new(ScrollbarSettings settings) {
    ElementHandle rail = Element_new((ElementSettings){
        .pos = settings.pos,
        .minWidth = settings.railWidth,
        .wantGrowVertical = true
    },
        Element_new((ElementSettings){
            pos
        })
    );


}

void handleSliderDragging(Element *element, Vec2f pos) {
}
