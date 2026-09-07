//
// Created by ertls on 07.09.2026.
//

#pragma once
#include "Typedef.h"

typedef enum ElementType: byte {
    ELEMENT_TYPE_DEFAULT,
    ELEMENT_TYPE_TEXTFIELD,
    ELEMENT_TYPE_SCROLLBAR_RAIL,
    ELEMENT_TYPE_SCROLLBAR_SLIDER,
    ELEMENT_TYPE_SCROLL_AREA
} ElementType;
