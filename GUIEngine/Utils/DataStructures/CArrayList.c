//
// Created by ertls on 28.08.2026.
//

#include "CArrayList.h"

#include <string.h>

#include "Utils/Typedef.h"
#include "Utils/Macros/Utils.h"

void _CArrayList_new(void** array, size_t typeSize, size_t capacity) {
    if(*array != nullptr) return;

    _Array_Header_* header = malloc(sizeof(_Array_Header_) + typeSize * capacity);
    assert(header != nullptr);
    header->capacity = capacity;
    header->size = 0;
    *array = (void*) (header+1);
}

void _CArrayList_resize(void **array, size_t typeSize, size_t newCapacity) {
    _Array_Header_* header = _CArrayList_getHeader(*array);
    _Array_Header_* newHeader = realloc(header, sizeof(_Array_Header_) + typeSize * newCapacity);

    if (!newHeader) ERROR_("Failed to realloc ArrayList");

    newHeader->capacity = newCapacity;
    newHeader->size = min(newHeader->size, newHeader->capacity);
    *array = (void *) (newHeader + 1);
}

void _CArrayList_copy(void** to, void** from, size_t typeSize) {
    if (*from == nullptr) return;
    if (*to == nullptr) _CArrayList_new(to, typeSize, ArrayInitCapacity);

    _Array_Header_* toHeader = _CArrayList_getHeader(*to);
    const _Array_Header_* fromHeader = _CArrayList_getHeader(*from);

    const size_t requiredCapacity = fromHeader->size;
    if (toHeader->capacity < requiredCapacity) _CArrayList_resize(to, typeSize, requiredCapacity);

    memcpy(toHeader, fromHeader, requiredCapacity * typeSize + sizeof(_Array_Header_));
}

void _CArrayList_erase(void** array, size_t typeSize, size_t index) {
    byte* ptr = (byte*)*array + index * typeSize;
    const size_t size = _CArrayList_getHeader(*array)->size;
    const size_t capacity = _CArrayList_getHeader(*array)->capacity;

    memmove(ptr, ptr + typeSize, size - index - 1);

    if (capacity > size * 2) _CArrayList_resize(array, typeSize, size);

    _CArrayList_getHeader(*array)->size--;
}

void _CArrayList_growIfNeededImpl(void **array, size_t typeSize) {
    const _Array_Header_* header = _CArrayList_getHeader(*array);
    if (header->capacity > header->size) return;
    _CArrayList_resize(array, typeSize, header->capacity * 2);
}

void _CArrayList_sort(void* array[], size_t typeSize) {

}

static void CArrayList_test() {
    int* array = {};

}