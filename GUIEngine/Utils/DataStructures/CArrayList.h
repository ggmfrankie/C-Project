//
// Created by ertls on 27.02.2026.
//

#pragma once
#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "../Logging/Logging.h"
#include "../Macros/Macros.h"

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

#define arrGet(array, index) ((array) != nullptr && _arrGetHead(array)->size > (index)) ? &(array)[index] : nullptr

#define arrPeek(array) (arrIsEmpty(array) ? nullptr : &(array)[arrLen(array)-1])

#define arrPop(array)\
({\
    if (arrIsEmpty(array)) ERROR_("Array does not contain any Items");\
    (array)[--_arrGetHead(array)->size];\
})

#define arrErase(array, index) do {\
    if (index >= _arrGetHead(array)->size) ERROR_("Index %llu out of Bounds for Array with size %llu", (uint64_t)(index), _arrGetHead(array)->size);\
    _arrErase((void**)&(array), sizeof(*array), index);\
} while(0)

/**
 * @brief copies the content from array to another array
 * @param to - the destination array
 * @param from - the source array
 * @warning content inside 'to' is overridden
 */
#define arrCopy(to, from) _arrCopy((void**)&to, (void**)&from, sizeof(*to))

/**
 * @brief clears array (sets its size to 0, capacity remains)
 * @param array - the array being cleared
 */
#define arrClear(array)\
    do {\
        if ((array) == nullptr) break;\
        _arrGetHead(array)->size = 0;\
    } while (0)

/**
 * @brief deletes array
 * @param array - the array being deleted
 */
#define arrFree(array) \
    do {\
        if((array) == nullptr) break;\
        free(_arrGetHead(array));\
        (array) = nullptr;\
    } while (0)

//@brief usage for arrEach(itemName, array) {...}
#define arrEach_impl(_end, item, array) (typeof(*(array))* item = (array), *_end = (array) + arrLen(array); (item) != _end; ++(item))
/**
 * @brief usage for arrEach(item, array) {...}
 * @param item - pointer to the current item in the array
 * @param array - the array being iterated over
 */
#define arrEach(item, array) arrEach_impl(CONCAT(_end, __COUNTER__), item, array)

/**
 * @brief usage for arrEachIdx(item, index, array) {...}
 * @param item - pointer to the current item in the array
 * @param index - current index
 * @param array - the array being iterated over
 * @warning break does not work, use goto
 */
#define arrEachIdx(item, index, array) (size_t index = 0, _end = arrLen(array); (index) < _end; ++(index)) \
              for (typeof(*(array))* (item) = &(array)[index]; (item); (item) = nullptr)

#define arrEachRev(item, array) (typeof(*(array))* item = (array) + arrLen(array), *_end = (array); (item)-- != _end;)

#define arrFindIf(item, array, ...)\
({\
typeof(array) CONCAT(_local, __LINE__) = nullptr;\
for arrEach(item, array) {\
    if (__VA_ARGS__) {\
        CONCAT(_local, __LINE__) = item;\
        break;\
    }\
}\
CONCAT(_local, __LINE__);\
})

#define arrRemoveIf(element, array, ...)\
do {\
    for arrEachIdx(element, CONCAT(_localIdx, __LINE__), array) {\
        if (__VA_ARGS__) {\
            arrErase(array, CONCAT(_localIdx, __LINE__));\
            break;\
        }\
    }\
} while (0)
