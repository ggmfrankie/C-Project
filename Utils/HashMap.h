//
// Created by Stefan on 17.10.2025.
//

#ifndef C_HASHMAP_H
#define C_HASHMAP_H
#define HASH_KEY 0
#include <stdint.h>
#include <string.h>
#include <_bsd_types.h>

#include "String.h"

typedef int Name;
typedef char* Key;
typedef int Value;

typedef struct MapEntry_Name {
    Key key;
    Value value;
    bool isOccupied;
} MapEntry_Name;

typedef struct Hashmap_Name {
    MapEntry_Name *content;
    size_t size;
    size_t capacity;

} Hashmap_Name;
static inline uint32_t FNV(const void* key, uint32_t h);


static inline Hashmap_Name createHashmap_Name(const size_t capacity) {
    MapEntry_Name* content = calloc(capacity, sizeof(MapEntry_Name));
    if (content) {
        puts("Memory allocation failed in createHashmap_Name");
        exit(-2);
    }
    return (Hashmap_Name){
        .content = content,
        .size = 0,
        .capacity = capacity,
    };
}

static inline void _resize(Hashmap_Name* map) {
    MapEntry_Name* oldContent = map->content;
    MapEntry_Name* newContent = calloc(map->capacity*2, sizeof(MapEntry_Name));
    const size_t oldCapacity = map->capacity;
    map->capacity *= 2;
    if (!newContent) {
        puts("Memory allocation failed in _resize");
        map->content = oldContent;
        return;
    }
    for (size_t s = 0; s < oldCapacity; s++) {
        const MapEntry_Name* entry = &oldContent[s];
        if (!entry->isOccupied) continue;
        size_t newIndex = FNV(entry->key, HASH_KEY) % map->capacity;

        bool isOccupied = newContent[newIndex].isOccupied;

        while (isOccupied) {
            newIndex = (newIndex + 1) % map->capacity;
            isOccupied = newContent[newIndex].isOccupied;
        }

        newContent[newIndex] = (MapEntry_Name){
            .key = entry->key,
            .value = entry->value,
            .isOccupied = entry->isOccupied,
        };
    }
    map->content = newContent;
    free(oldContent);
}

static inline void Hashmap_Name_add(Hashmap_Name* map, const Key key, const Value value) {
    if ((float)map->size / (float)map->capacity > 0.75f) _resize(map);

    const uint32_t hash = FNV(&key, HASH_KEY);
    size_t index = hash % map->size;
    bool isOccupied = map->content[index].isOccupied;

    while (isOccupied) {
        index = (index + 1) % map->capacity;
        isOccupied = map->content[index].isOccupied;
    }
    map->content[index] = (MapEntry_Name){
        .key = key,
        .value = value,
        .isOccupied = true,
    };
}

static inline Type* Hashmap_Name_get() {

}


static inline uint32_t FNV(const void* key, uint32_t h){
    h ^= 2166136261UL;
    const uint8_t* data = (const uint8_t*)key;
    for(int i = 0; data[i] != '\0'; i++)
    {
        h ^= data[i];
        h *= 16777619;
    }
    return h;
}



#endif //C_HASHMAP_H