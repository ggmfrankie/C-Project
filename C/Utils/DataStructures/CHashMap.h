//
// Created by Stefan on 14.05.2026.
//

#pragma once
#include "Utils/Typedef.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
} _Map_Header_;

void _mapNew(void** map, size_t typeSize, size_t capacity);
void _mapResize(void** map, size_t typeSize, size_t newCapacity);

uint32_t _mapHash(const char* key);
void* _mapGet(void* map, size_t typeSize, const char* key);
void _mapFree(void** map);

void _mapGrowIfNeededImpl(void** map, size_t typeSize);

void* _mapGetEmptySlotImpl(void* map, size_t typeSize, const char* key);

#define _MapInitCapacity 256
#define _MapSeed 342341431UL
#define _mapContentSize(map) (sizeof(*(map)) + sizeof(char*))

#define _mapGetHead(map) (&((_Map_Header_*)(map))[-1])
#define _mapKey(_place) (*(const char**)(_place))
#define _mapValue(map, _place) ((typeof(map))((byte*)(_place) + sizeof(char*)))
#define _mapGrowIfNeeded(map) _mapGrowIfNeededImpl((void**)&(map), _mapContentSize(map))
#define _mapGetEmptySlot(map, key) _mapGetEmptySlotImpl((void**)&(map), _mapContentSize(map), (key))

#define mapNew(map, size) _mapNew((void**)&(map), _mapContentSize(map), (size))
#define mapLen(map) ((map) ? (_mapGetHead(map)->size) : 0)
#define mapCap(map) ((map) ? (_mapGetHead(map)->capacity) : 0)
#define mapGet(map, key) (typeof(map)) _mapGet((void*)(map), _mapContentSize(map), (key))
#define mapFree(map) _mapFree(&(map))

#define mapIsEmpty(map) (arrLen(map) == 0)

#define mapInsert(map, key, value)\
    do {\
        if ((map) == nullptr) mapNew(map, _MapInitCapacity);\
        _mapGrowIfNeeded(map);\
        *(typeof(map))_mapGetEmptySlot(map, key) = value;\
        _mapGetHead(map)->size++;\
    } while (0)
