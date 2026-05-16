//
// Created by ertls on 27.02.2026.
//

#pragma once
#include <assert.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
} __Array_Header_;

#define ArrayInitCapacity 16

#define _arrayGetHead(array) (((__Array_Header_*)(array))[-1])

#define arrLen(array) ((array) ? _arrayGetHead(array).size : 0)

#define arrIsEmpty(array) (arrLen(array) == 0)

#define arrPush(array, x) \
    do {\
        if((array) == nullptr) {\
            __Array_Header_* header = malloc(sizeof(*(array)) * ArrayInitCapacity + sizeof(__Array_Header_));\
            assert(header != nullptr);\
            header->capacity = ArrayInitCapacity;\
            header->size = 0;\
            (array) = (void*) (header+1);\
        }\
        __Array_Header_* header = &_arrayGetHead(array);\
        if (header->capacity <= header->size) {\
            assert(header->capacity != 0);\
            const size_t newCapacity = header->capacity * 2;\
            __Array_Header_* newHeader = realloc(header, sizeof(__Array_Header_) + sizeof(*(array)) * newCapacity);\
            if (newHeader) {\
                header = newHeader;\
                header->capacity = newCapacity;\
                (array) = (void*) (header+1);\
            } else {\
                puts("Failed to realloc ArrayList");\
            }\
        }\
        (array)[header->size++] = x;\
    } while (0)

#define arrTryGet(array, index) (((array) == nullptr) ? nullptr : (_arrayGetHead(array).size <= (index)) ? nullptr : &(array)[index])

#define arrGetLast(array) (array) ? &(array)[arrLen(array)-1]: nullptr;

#define arrClear(array)\
    do {\
        if ((array) == nullptr) break;\
        _arrayGetHead(array).size = 0;\
    } while (0)

#define arrDel(array) \
    do {\
        assert((array) != nullptr);\
        free(&_arrayGetHead(array));\
        (array) = nullptr;\
    } while (0)

#define for_eachArr(item, array, ...)\
    do {\
        if ((array) == nullptr) break;\
       \
        size_t len = arrLen(array);\
        for (size_t i = 0; i < len; ++i) {\
            auto (item) = &(array)[i];\
            __VA_ARGS__\
        }\
    } while (0)

#define for_eachRevArr(item, array, ...)\
    do { \
        if ((array) == nullptr) break; \
        size_t len = arrLen(array); \
        for (size_t i = len; i --> 0;) { \
            auto item = &(array)[i]; \
            __VA_ARGS__ \
        } \
    } while (0)


