//
// Created by Stefan on 06.09.2026.
//

#include "ScrollArea.h"

#include "Scrollbar.h"

ElementHandle _ScrollArea_new(ScrollAreaSettings settings, ...) {
    const ElementHandle frame = Element_new((ElementSettings){
        .pos = settings.pos,
        .minWidth = settings.width,
        .minHeight = settings.height,
        .clipArea = {.pos = {0,0}, .dims = {settings.width, settings.height}}
    });

    const ElementHandle scrollbar = Scrollbar_new((ScrollbarSettings){
        .pos = {},
        .railWidth = 10,
        .sliderHeight = 20
    });

    const ElementHandle panel = Element_new((ElementSettings){
        .pos = settings.pos,
        .minWidth = settings.width,
        .invisible = true,
        .posMode = POS_RELATIVE,
        .noLayoutContribution = true
    },
        scrollbar,
        frame
    );

    va_list args;
    va_start(args, settings);

    Element_addChildElements_vaList(panel, args);

    va_end(args);

    return frame;
}
