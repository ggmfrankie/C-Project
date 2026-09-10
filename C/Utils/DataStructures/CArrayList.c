//
// Created by ertls on 28.08.2026.
//

#include "CArrayList.h"

#include <string.h>

#include "Typedef.h"
#include "Makros/Makros.h"

void _arrNew(void** array, size_t typeSize, size_t capacity) {
    if(*array != nullptr) return;

    _Array_Header_* header = malloc(sizeof(_Array_Header_) + typeSize * capacity);
    assert(header != nullptr);
    header->capacity = capacity;
    header->size = 0;
    *array = (void*) (header+1);
}

void _arrResize(void **array, size_t typeSize, size_t newCapacity) {
    _Array_Header_* header = _arrGetHead(*array);
    _Array_Header_* newHeader = realloc(header, sizeof(_Array_Header_) + typeSize * newCapacity);

    if (!newHeader) ERROR_("Failed to realloc ArrayList");

    newHeader->capacity = newCapacity;
    *array = (void *) (newHeader + 1);
}

void _arrCopy(void** to, void** from, size_t typeSize) {
    if (*to == nullptr) _arrNew(to, typeSize, ArrayInitCapacity);

    _Array_Header_* toHeader = _arrGetHead(*to);
    const _Array_Header_* fromHeader = _arrGetHead(*from);

    const size_t requiredCapacity = fromHeader->size;
    if (toHeader->capacity < requiredCapacity) _arrResize(to, typeSize, requiredCapacity);

    memcpy(toHeader, fromHeader, requiredCapacity * typeSize + sizeof(_Array_Header_));
}

void _arrErase(void** array, size_t typeSize, size_t index) {
    byte* ptr = (byte*)*array + index * typeSize;
    const size_t size = _arrGetHead(*array)->size;
    const size_t capacity = _arrGetHead(*array)->capacity;

    memmove(ptr, ptr + typeSize, size - index - 1);

    if (capacity > size * 2) _arrResize(array, typeSize, size);

    _arrGetHead(*array)->size--;
}

void _arrGrowIfNeededImpl(void **array, size_t typeSize) {
    const _Array_Header_* header = _arrGetHead(*array);
    if (header->capacity > header->size) return;
    _arrResize(array, typeSize, header->capacity * 2);
}
