//
// Created by ertls on 27.02.2026.
//

#pragma once
#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "../Logging/Logging.h"
#include "../Makros/Helper.h"

typedef struct {
    size_t size;
    size_t capacity;
} _Array_Header_;

void _arrNew(void** array, size_t typeSize, size_t capacity);
void _arrResize(void **array, size_t typeSize, size_t newCapacity);
void _arrCopy(void** to, void** from, size_t typeSize);
void _arrErase(void** array, size_t typeSize, size_t index);

void _arrGrowIfNeededImpl(void **array, size_t typeSize);

#define ArrayInitCapacity 16

#define _arrGetHead(array) (&((_Array_Header_*)(array))[-1])
#define _arrGrowIfNeeded(array) _arrGrowIfNeededImpl((void**)&(array), sizeof(*(array)))

#define arrNew(array, size) _arrNew((void**)&(array), sizeof(*array), (size))
#define arrLen(array) ((array) ? _arrGetHead(array)->size : 0)
#define arrCap(array) ((array) ? _arrGetHead(array)->capacity : 0)
#define arrIsEmpty(array) (arrLen(array) == 0)

#define arrPush(array, item) \
    do {\
        if((array) == nullptr) arrNew((array), ArrayInitCapacity);\
        _arrGrowIfNeeded(array);\
        (array)[_arrGetHead(array)->size++] = (item);\
    } while (0)

#define arrGet(array, index) ((array) != nullptr && _arrGetHead(array)->size > (index)) ? &(array)[index] : nullptr;

#define arrPeek(array) (arrIsEmpty(array) ? nullptr : &(array)[arrLen(array)-1])

#define arrPop(array)\
({\
    if (arrIsEmpty(array)) ERROR_("Array does not contain any Items");\
    (array)[--_arrGetHead(array)->size];\
})

#define arrErase(array, index, destructor) do {\
    if (index >= _arrGetHead(array)->size) ERROR_("Index %llu out of Bounds for Array with size %llu", index, _arrGetHead(array)->size)\
    if (destructor) destructor(&array[index]);\
    _arrErase(array, sizeof(*array), index);\
} while(0)

#define arrCopy(to, from) _arrCopy((void**)&to, (void**)&from, sizeof(*to))

#define arrClear(array)\
    do {\
        if ((array) == nullptr) break;\
        _arrGetHead(array)->size = 0;\
    } while (0)

#define arrFree(array) \
    do {\
        if((array) == nullptr) break;\
        free(_arrGetHead(array));\
        (array) = nullptr;\
    } while (0)

//@brief usage for arrEach(itemName, array) {...}
#define arrEach(item, array) (typeof(*(array))* item = (array), *_end = (array) + arrLen(array); (item) != _end; ++(item))
//@brief usage for arrEach(itemName, indexName, array) {...}
#define arrEachIdx(item, index, array) (size_t (index) = 0, _end = arrLen(array); (index) < _end; ++(index)) \
              for (typeof(*(array))* (item) = &(array)[index]; (item); (item) = nullptr)
#define arrEachRev(item, array) (typeof(*(array))* item = (array) + arrLen(array), *_end = (array); (item)-- != _end;)

#define arrContains(array, item) ({\
    bool CONCAT(_local, __LINE__) = false;\
    for_eachArr(value, array, {\
        if (*value == item) {\
            CONCAT(_local, __LINE__) = true;\
            break;\
        }\
    });\
    CONCAT(_local, __LINE__);\
})

