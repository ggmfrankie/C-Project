//
// Created by Stefan on 14.05.2026.
//
#include "CHashMap.h"

#include <assert.h>
#include <string.h>
#include "Utils/Logging/Logging.h"

void _mapNew(void** map, size_t typeSize, size_t capacity) {
    if (*map != nullptr) return;

    _Map_Header_* header = calloc(1, sizeof(_Map_Header_) + typeSize * capacity);
    if (!header) ERROR_("Hashmap allocation failed");

    header->capacity = capacity;
    header->size = 0;
    *map = (void*) (header+1);
}

void _mapResize(void** map, size_t typeSize, size_t newCapacity) {
    _Map_Header_* header = _mapGetHead(*map);

    _Map_Header_* newHeader = calloc(1, sizeof(_Map_Header_) + typeSize * newCapacity);
    const auto newContent = (byte*)(newHeader+1);

    if (!newContent) ERROR_("HashMap resize failed");

    int i = 0;
    for (auto oldSlot = (byte*) *map; i < header->capacity; oldSlot += typeSize, ++i) {
        const char* _key_ = _mapKey(oldSlot);
        if (_key_ == nullptr) continue;

        byte* newSlot = newContent + _mapHash(_key_) % newCapacity * typeSize;

        const byte* _end = newContent + typeSize * newCapacity;
        while (_mapKey(newSlot) != nullptr) {
            newSlot += typeSize;
            if (newSlot >= _end) {
                newSlot = newContent;
            }
        }
        memcpy(newSlot, oldSlot, typeSize);
    }
    newHeader->size = header->size;
    newHeader->capacity = newCapacity;
    free(header);
    *map = (void*) (newHeader+1);
}

uint32_t _mapHash(const char* key) {
    assert(key != nullptr);
    uint32_t h = _MapSeed;
    h ^= 2166136261UL;
    for(int i = 0; (unsigned char)key[i] != '\0'; ++i) {
        h ^= (unsigned char)key[i];
        h *= 16777619;
    }
    return h;
}

void* _mapGet(void* map, size_t typeSize, const char* key) {
    if (map == nullptr || key == nullptr) return nullptr;

    const _Map_Header_* header = _mapGetHead(map);
    const size_t capacity = header->capacity;
    const size_t index = _mapHash(key) % capacity;

    byte* slot = (byte*)map + index * typeSize;
    const byte* end = (byte*)map + typeSize * capacity;

    while (_mapKey(slot) != nullptr) {
        if (strcmp(_mapKey(slot), key) == 0) return slot + sizeof(char*);
        slot += typeSize;
        if (slot >= end) slot = map;
    }

    WARNING_("Key is not inside the hashmap");
    return nullptr;
}

void _mapFree(void** map) {
    if (*map == nullptr) return;
    free(_mapGetHead(*map));
}

void _mapGrowIfNeededImpl(void** map, size_t typeSize) {
    const _Map_Header_* header = _mapGetHead(*map);
    if (header->capacity * 0.7 > header->size) return;
    _mapResize(map, typeSize, header->capacity * 2);
}

void* _mapGetEmptySlotImpl(void* map, size_t typeSize, const char* key) {
    if (map == nullptr || key == nullptr) return nullptr;

    const _Map_Header_* header = _mapGetHead(map);
    const size_t capacity = header->capacity;
    const size_t index = _mapHash(key) % capacity;

    byte* slot = (byte*)map + index * typeSize;

    const byte* end = (byte*)map + typeSize * capacity;

    while (_mapKey(slot) != nullptr) {
        if (strcmp(_mapKey(slot), key) == 0) return nullptr;
        slot += typeSize;
        if (slot >= end) slot = map;
    }
    _mapKey(slot) = key;
    return slot + sizeof(char*);
}
