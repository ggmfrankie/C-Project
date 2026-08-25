#include "Scrollbar.h"

ElementHandle Scrollbar_new(ScrollbarSettings settings) {
    ElementHandle rail = Element_new((ElementSettings){
        .pos = settings.pos,
        .minWidth = settings.railWidth,
        .wantGrowVertical = true
    },
        Element_new((ElementSettings){
            .pos = {0, 0}
        })
    );


}

void Scrollbar_handleDragging(Element *element, Vec2f pos) {
}
