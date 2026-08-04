//
// Created by Stefan on 14.05.2026.
//
#include "CHashMap.h"
#include <string.h>
#include "Utils/Logging/Logging.h"

uint32_t _hashMapHash_(const char* key) {
    assert(key != nullptr);
    uint32_t h = _HashMapSeed_;
    h ^= 2166136261UL;
    const uint8_t* data = (const uint8_t*)key;
    for(int i = 0; data[i] != '\0'; ++i) {
        h ^= data[i];
        h *= 16777619;
    }
    return h;
}

void* _hashMapGet_(byte* map, const char* key, size_t stride) {
    if (map == nullptr || key == nullptr) return nullptr;

    const _HashMap_Header_* header = &_hashMapGetHead(map);
    const size_t capacity = header->capacity;

    byte* place = map + (_hashMapHash_(key) % capacity) * stride;

    const byte* end = map + stride * capacity;

    for (int i = 0; i < capacity; ++i) {
        const char* presentKey = _hashMapKey(place);
        if (presentKey == nullptr) {
            puts("Key is not inside the hashmap");
            return nullptr;
        }
        if (strcmp(presentKey, key) == 0) return place + sizeof(char*);

        place += stride;
        if (place >= end) place = map;
    }
    WARNING("Hashmap full???");
    return nullptr;
}