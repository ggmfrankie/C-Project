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

void* _arrInit(void **array, size_t typeSize, size_t size);
void _arrResize(void **array, size_t typeSize, size_t newSize);

void _arrGrowIfNeeded(void **array, size_t typeSize);

#define ArrayInitCapacity 16

#define _arrayGetHead(array) (&((_Array_Header_*)(array))[-1])

#define arrNew(array, size) _arrInit((void**)&(array), sizeof(*array), (size))

#define arrLen(array) ((array) ? _arrayGetHead(array)->size : 0)

#define arrIsEmpty(array) (arrLen(array) == 0)

#define arrPush(array, item) \
    do {\
        if((array) == nullptr) arrNew((array), ArrayInitCapacity);\
        _arrGrowIfNeeded((void**)&(array), sizeof(*(array)));\
        (array)[_arrayGetHead(array)->size++] = (item);\
    } while (0)

#define arrTryGet(array, index)\
    ({\
        typeof(array) out = nullptr;\
        if ((array) != nullptr && _arrayGetHead(array)->size > (index)) out = &(array)[index];\
        (typeof(array))out;\
    })

#define arrGetLast(array)\
({\
    typeof(array) CONCAT(_local, __LINE__) = nullptr;\
    if (!arrIsEmpty(array)) {\
        CONCAT(_local, __LINE__) = &(array)[arrLen(array)-1];\
    }\
    CONCAT(_local, __LINE__);\
})

#define arrPop(array)\
({\
    if (array == nullptr || arrIsEmpty(array)) ERROR_("Array does not contain any Items");\
    _arrayGetHead(array)->size--;\
    (array)[arrLen(array)];\
})

#define arrClear(array)\
    do {\
        if ((array) == nullptr) break;\
        _arrayGetHead(array)->size = 0;\
    } while (0)

#define arrFree(array) \
    do {\
        if((array) == nullptr) break;\
        free(_arrayGetHead(array));\
        (array) = nullptr;\
    } while (0)

//@brief usage for arrEach(itemName, array) {...}
#define arrEach(item, array) (typeof(*(array))* item = (array), *_end = (array) + arrLen(array); (item) != _end; ++(item))
//@brief usage for arrEach(itemName, indexName, array) {...}
#define arrEachIdx(item, index, array) (size_t (index) = 0, _end = arrLen(array); (index) < _end; ++(index)) for (typeof(*(array))* (item) = &(array)[index]; (item); (item) = nullptr)
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

