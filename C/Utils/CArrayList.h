//
// Created by ertls on 27.02.2026.
//

#ifndef MIXEDPROJECT_CARRAYLIST_H
#define MIXEDPROJECT_CARRAYLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

    typedef struct {
        size_t size;
        size_t capacity;
    } Array_Header;

#define _initArray(x)

    inline int* initArray(const size_t capacity) {
        Array_Header* header = malloc(sizeof(int)*capacity + sizeof(Array_Header));
        header->capacity = capacity;
        header->size = 0;

        return (int*) header+1;
    }

    inline void arrayPush(int* arr, const int x) {
        Array_Header* header = ((Array_Header*) arr)-1;
        if (header->capacity <= header->size) {
            const size_t newCapacity = header->capacity*2 + sizeof(Array_Header);
            Array_Header* newHeader = realloc(header, newCapacity);
            if (newHeader) {
                header = newHeader;
                header->capacity = newCapacity;
                arr = (int*) header+1;
            }
        }
        arr[header->size++] = x;
    }

#ifdef __cplusplus
}
#endif

#endif //MIXEDPROJECT_CARRAYLIST_H