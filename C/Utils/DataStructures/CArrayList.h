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

#define ArrayInitCapacity 16

#define _arrayGetHead(array) (&((_Array_Header_*)(array))[-1])

#define arrLen(array) ((array) ? _arrayGetHead(array)->size : 0)

#define arrIsEmpty(array) (arrLen(array) == 0)

#define arrPush(array, ...) \
    do {\
        if((array) == nullptr) {\
            _Array_Header_* header = malloc(sizeof(*(array)) * ArrayInitCapacity + sizeof(_Array_Header_));\
            assert(header != nullptr);\
            header->capacity = ArrayInitCapacity;\
            header->size = 0;\
            (array) = (void*) (header+1);\
        }\
        _Array_Header_* header = _arrayGetHead(array);\
        if (header->capacity <= header->size) {\
            assert(header->capacity != 0);\
            const size_t newCapacity = header->capacity * 2;\
            _Array_Header_* newHeader = realloc(header, sizeof(_Array_Header_) + sizeof(*(array)) * newCapacity);\
            if (newHeader) {\
                header = newHeader;\
                header->capacity = newCapacity;\
                (array) = (void*) (header+1);\
            } else {\
                ERROR_("Failed to realloc ArrayList");\
            }\
        }\
        (array)[header->size++] = __VA_ARGS__;\
    } while (0)

#define arrTryGet(array, index)\
    ({\
        typeof(array) out = nullptr;\
        if ((array) != nullptr && _arrayGetHead(array)->size > (index)) out = &(array)[index];\
        (typeof(array))out;\
    })

#define arrGetLast(array) ((array) ? &(array)[arrLen(array)-1]: nullptr)

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

#define arrDelete(array) \
    do {\
        if((array) == nullptr) break;\
        free(_arrayGetHead(array));\
        (array) = nullptr;\
    } while (0)

#define for_eachArr(item, array, ...)\
    do {\
        if ((array) == nullptr) break;\
       \
        size_t len = arrLen(array);\
        for (size_t i = 0; i < len; ++i) {\
            typeof(array) (item) = &(array)[i];\
            __VA_ARGS__\
        }\
    } while (0)

#define for_eachRevArr(item, array, ...)\
    do { \
        if ((array) == nullptr) break; \
        size_t len = arrLen(array); \
        for (size_t i = len; i --> 0;) { \
            typeof(array) item = &(array)[i]; \
            __VA_ARGS__ \
        } \
    } while (0)

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

