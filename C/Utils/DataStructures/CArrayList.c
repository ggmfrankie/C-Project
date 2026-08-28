//
// Created by ertls on 28.08.2026.
//

#include "CArrayList.h"

void* _arrInit(void **array, size_t typeSize, size_t size) {
    if(*array != nullptr) return *array;

    _Array_Header_* header = malloc(sizeof(_Array_Header_) + typeSize * size);
    assert(header != nullptr);
    header->capacity = size;
    header->size = 0;
    *array = (void*) (header+1);
    return *array;
}

void _arrResize(void **array, size_t typeSize, size_t newSize) {
    _Array_Header_* header = _arrayGetHead(*array);
    assert(header->capacity != 0);
    _Array_Header_ *newHeader = realloc(header, sizeof(_Array_Header_) + typeSize * newSize);

    if (!newHeader) ERROR_("Failed to realloc ArrayList");

    header = newHeader;
    header->capacity = newSize;
    *array = (void *) (header + 1);
}

void _arrGrowIfNeeded(void **array, size_t typeSize) {
    const _Array_Header_* header = _arrayGetHead(*array);

    if (header->capacity > header->size) return;
    _arrResize(array, typeSize, header->capacity * 2);
}
