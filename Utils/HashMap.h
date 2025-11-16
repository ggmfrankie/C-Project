//
// Created by Stefan on 17.10.2025.
//

#ifndef C_HASHMAP_H
#define C_HASHMAP_H
#include <stdint.h>

#include "String.h"

typedef int Name;
typedef String Key;
typedef int Value;

typedef struct MapEntry_Name {
    Key key;
    Value value;
} MapEntry_Name;

typedef struct Hashmap_Name {
    MapEntry_Name *content;
    size_t size;

} Hashmap_Name;

static uint32_t FNV(const void* key, uint32_t h){
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