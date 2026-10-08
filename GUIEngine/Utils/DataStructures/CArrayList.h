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

#define CARRAY_LIST_SHORT_NAMES
#ifdef CARRAY_LIST_SHORT_NAMES

#define arrNew     CArrayList_new
#define arrClear   CArrayList_clear
#define arrFree    CArrayList_free
#define arrCopy    CArrayList_copy

#define arrLen     CArrayList_len
#define arrCap     CArrayList_cap
#define arrIsEmpty CArrayList_isEmpty

#define arrPush    CArrayList_push
#define arrGet     CArrayList_get
#define arrPeek    CArrayList_peek
#define arrPop     CArrayList_pop
#define arrErase   CArrayList_erase

#define arrEach    CArrayList_each
#define arrEachIdx CArrayList_eachIdx
#define arrEachRev CArrayList_eachRev
#define arrFindIf  CArrayList_findIf
#define arrEraseIf CArrayList_eraseIf
#endif

typedef struct {
    size_t size;
    size_t capacity;
} _Array_Header_;

void _CArrayList_new(void** array, size_t typeSize, size_t capacity);
void _CArrayList_resize(void **array, size_t typeSize, size_t newCapacity);
void _CArrayList_copy(void** to, void** from, size_t typeSize);
void _CArrayList_erase(void** array, size_t typeSize, size_t index);

void _CArrayList_growIfNeededImpl(void **array, size_t typeSize);

#define ArrayInitCapacity 16

#define _CArrayList_getHeader(array) (&((_Array_Header_*)(array))[-1])
#define _CArrayList_growIfNeeded(array) _CArrayList_growIfNeededImpl((void**)&(array), sizeof(*(array)))

#define CArrayList_new(array, size) _CArrayList_new((void**)&(array), sizeof(*array), (size))
#define CArrayList_len(array) ((array) ? _CArrayList_getHeader(array)->size : 0)
#define CArrayList_cap(array) ((array) ? _CArrayList_getHeader(array)->capacity : 0)
#define CArrayList_isEmpty(array) (CArrayList_len(array) == 0)

#define CArrayList_push(array, item) \
    do {\
        if((array) == nullptr) CArrayList_new((array), ArrayInitCapacity);\
        _CArrayList_growIfNeeded(array);\
        (array)[_CArrayList_getHeader(array)->size++] = (item);\
    } while (0)

#define CArrayList_get(array, index) ((array) != nullptr && _CArrayList_getHeader(array)->size > (index)) ? &(array)[index] : nullptr

#define CArrayList_peek(array) (CArrayList_isEmpty(array) ? nullptr : &(array)[CArrayList_len(array)-1])

#define CArrayList_pop(array)\
({\
    if (CArrayList_isEmpty(array)) ERROR_("Array does not contain any Items");\
    (array)[--_CArrayList_getHeader(array)->size];\
})

#define CArrayList_erase(array, index) do {\
    if (index >= _CArrayList_getHeader(array)->size) ERROR_("Index %lu out of Bounds for Array with size %lu", (uint64_t)(index), _CArrayList_getHeader(array)->size);\
    _CArrayList_erase((void**)&(array), sizeof(*array), index);\
} while(0)

/**
 * @brief copies the content from array to another array
 * @param to - the destination array
 * @param from - the source array
 * @warning content inside 'to' is overridden
 */
#define CArrayList_copy(to, from) _CArrayList_copy((void**)&to, (void**)&from, sizeof(*to))

/**
 * @brief clears array (sets its size to 0, capacity remains)
 * @param array - the array being cleared
 */
#define CArrayList_clear(array)\
    do {\
        if ((array) == nullptr) break;\
        _CArrayList_getHeader(array)->size = 0;\
    } while (0)

/**
 * @brief deletes array
 * @param array - the array being deleted
 */
#define CArrayList_free(array) \
    do {\
        if((array) == nullptr) break;\
        free(_CArrayList_getHeader(array));\
        (array) = nullptr;\
    } while (0)

#define CArrayList_sort(array)


//@brief usage for CArrayList_each(itemName, array) {...}
#define CArrayList_each_impl(_end, item, array) (typeof(*(array))* item = (array), *_end = (array) + CArrayList_len(array); (item) != _end; ++(item))
/**
 * @brief usage for CArrayList_each(item, array) {...}
 * @param item - pointer to the current item in the array
 * @param array - the array being iterated over
 */
#define CArrayList_each(item, array) CArrayList_each_impl(CONCAT(_end, __COUNTER__), item, array)

/**
 * @brief usage for CArrayList_eachIdx(item, index, array) {...}
 * @param item - pointer to the current item in the array
 * @param index - current index
 * @param array - the array being iterated over
 * @warning break does not work, use goto
 */
#define CArrayList_eachIdx(item, index, array) (size_t index = 0, _end = CArrayList_len(array); (index) < _end; ++(index)) \
              for (typeof(*(array))* (item) = &(array)[index]; (item); (item) = nullptr)

#define CArrayList_eachRev(item, array) (typeof(*(array))* item = (array) + CArrayList_len(array), *_end = (array); (item)-- != _end;)

#define CArrayList_findIf(item, array, ...)\
({\
typeof(array) CONCAT(_local, __LINE__) = nullptr;\
for CArrayList_each(item, array) {\
    if (__VA_ARGS__) {\
        CONCAT(_local, __LINE__) = item;\
        break;\
    }\
}\
CONCAT(_local, __LINE__);\
})

#define CArrayList_eraseIf(item, array, ...)\
do {\
    for CArrayList_eachIdx(item, CONCAT(_localIdx, __LINE__), array) {\
        if (__VA_ARGS__) {\
            CArrayList_erase(array, CONCAT(_localIdx, __LINE__));\
            break;\
        }\
    }\
} while (0)
