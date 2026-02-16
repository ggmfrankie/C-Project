//
// Created by Stefan on 17.10.2025.
//

#ifndef C_HASHMAP_H
#define C_HASHMAP_H
#define HASH_KEY 0
#include <stdint.h>
#include <string.h>
#include <_bsd_types.h>

#include "CString.h"


#define HASH_MAP(Name, Key, Value)\
typedef struct MapEntry_##Name {\
    const Key key;\
    Value value;\
    bool isOccupied;\
} MapEntry_##Name;\
\
typedef struct Hashmap_##Name {\
    MapEntry_##Name *content;\
    size_t size;\
    size_t capacity;\
\
} Hashmap_##Name;\
\
static inline Hashmap_##Name newHashmap_##Name(const size_t capacity) {\
    MapEntry_##Name* content = calloc(capacity, sizeof(MapEntry_##Name));\
    if (!content) {\
        puts("Memory allocation failed in newHashmap_Name");\
        exit(-2);\
    }\
    return (Hashmap_##Name){\
        .content = content,\
        .size = 0,\
        .capacity = capacity,\
    };\
}\
\
static inline uint32_t FNV_##Name(const void* key, uint32_t h){\
    h ^= 2166136261UL;\
    const uint8_t* data = (const uint8_t*)key;\
    for(int i = 0; data[i] != '\0'; i++)\
    {\
        h ^= data[i];\
        h *= 16777619;\
    }\
    return h;\
}\
\
static inline void _resize_##Name(Hashmap_##Name* map) {\
    MapEntry_##Name* oldContent = map->content;\
    MapEntry_##Name* newContent = calloc(map->capacity*2, sizeof(MapEntry_##Name));\
    const size_t oldCapacity = map->capacity;\
    map->capacity *= 2;\
    if (!newContent) {\
        puts("Memory allocation failed in _resize");\
        map->content = oldContent;\
        return;\
    }\
    for (size_t s = 0; s < oldCapacity; s++) {\
        const MapEntry_##Name* entry = &oldContent[s];\
        if (!entry->isOccupied) continue;\
        size_t newIndex = FNV_##Name(entry->key, 0) % map->capacity;\
\
        bool isOccupied = newContent[newIndex].isOccupied;\
\
        while (isOccupied) {\
            newIndex = (newIndex + 1) % map->capacity;\
            isOccupied = newContent[newIndex].isOccupied;\
        }\
\
        newContent[newIndex] = (MapEntry_##Name){\
            .key = entry->key,\
            .value = entry->value,\
            .isOccupied = entry->isOccupied,\
        };\
    }\
    map->content = newContent;\
    free(oldContent);\
}\
\
static inline void Hashmap_##Name##_add(Hashmap_##Name* map, const Key key, Value value) {\
    if ((float)map->size+1 / (float)map->capacity > 0.75f) _resize_##Name(map);\
\
    const uint32_t hash = FNV_##Name(key, 0);\
    size_t index = hash % map->capacity;\
    bool isOccupied = map->content[index].isOccupied;\
\
    while (isOccupied) {\
        index = (index + 1) % map->capacity;\
        isOccupied = map->content[index].isOccupied;\
    }\
    map->content[index] = (MapEntry_##Name){\
        .key = key,\
        .value = value,\
        .isOccupied = true,\
    };\
}\
\
static inline Value* Hashmap_##Name##_get(const Hashmap_##Name* map, const Key key) {\
    size_t index = FNV_##Name(key, 0) % map->capacity;\
    if (!map->content[index].isOccupied) return NULL;\
    size_t iterations = 0;\
    while (strcmp(map->content[index].key, key)!= 0){\
        index = (index + 1) % map->capacity;\
        iterations++;\
        if (iterations >= map->capacity || !map->content[index].isOccupied) return NULL;\
    }\
    return &map->content[index].value;\
}



#endif //C_HASHMAP_H