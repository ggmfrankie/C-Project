//
// Created by Stefan on 27.09.2026.
//

#include "CHashTable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils/Math/Vector.h"

#define CHashTable_Seed 342341431UL

uint32_t CHashTable_hash(const byte* key, size_t len) {
    assert(key != nullptr);
    uint32_t h = CHashTable_Seed;
    h ^= 2166136261UL;
    for(int i = 0; i < len; ++i) {
        h ^= key[i];
        h *= 16777619;
    }
    return h;
}

void _CHashTable_new(void **table, size_t typeSize, size_t capacity) {
    CHashTable_Header* header = malloc(sizeof(CHashTable_Header) + typeSize * capacity);
    assert(header != nullptr);

    header->data.capacity = capacity;
    header->size = 0;

    const size_t tableCapacity = capacity * 4;
    ssize_t* indices = malloc(sizeof(*indices) * tableCapacity);
    assert(indices != nullptr);
    memset(indices, -1, tableCapacity);

    header->table.data = indices;
    header->table.capacity = tableCapacity;

    *table = (void*) (header+1);
    puts("Initialized Table");
}

void* _CHashTable_get(void *hashTable, const byte *keyData, size_t keySize, size_t typeSize) {
    assert(hashTable != nullptr && keyData != nullptr);
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    const ssize_t* indices = header->table.data;
    const size_t capacity = header->table.capacity;

    const ssize_t hash = CHashTable_hash(keyData, keySize);

    printf("hash=%zu slot=%zu\n", hash, hash % capacity);

    const ssize_t* it = indices + hash % capacity;
    printf(
        "Slot: %p, index: %zd\n",
        (void *) it,
        *it
    );
    const ssize_t* end = indices + capacity;

    while (*it != -1) {
        // Key at the iterator equals the provided key
        void* slot = (byte*)hashTable + (*it) * typeSize;
        if (memcmp(slot, keyData, keySize) == 0) return slot;
        ++it;
        // Wrap around at the end
        if(it == end) it = indices;
    }

    return nullptr;
}

ssize_t* _CHashTable_getFreeIndex(void* hashTable, const byte *keyData, size_t keySize) {
    assert(hashTable != nullptr && data != nullptr && len != 0);
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    ssize_t* indices = header->table.data;
    const size_t capacity = header->table.capacity;

    const ssize_t hash = CHashTable_hash(keyData, keySize);

    printf("hash=%zu slot=%zu\n", hash, hash % capacity);

    ssize_t* it = indices + hash  % capacity;
    const ssize_t* end = indices + capacity;

    while (*it != -1) {
        // Key at the iterator equals the provided key
        ++it;
        // Wrap around at the end
        if(it == end) it = indices;
    }

    printf(
        "Free slot: %p, current value: %zd\n",
        (void *) it,
        *it
    );
    return it;
}

void CHashTable_growIfNeeded(void **hashTable, uint32_t typeSize, uint32_t keySize, bool isString) {
    CHashTable_Header* header = _CHashTable_getHeader(*hashTable);

    if ((float)header->size / header->table.capacity > 0.75) {
        const size_t newCapacity = header->table.capacity *2;

        ssize_t* newIndices = realloc(header->table.data, newCapacity);
        memset(newIndices, -1, newCapacity);
        assert(newIndices != nullptr);

        header->table.data = newIndices;
        header->table.capacity = newCapacity;

        for (int i = 0; i < header->size; ++i) {
            const void* keyPtr = (byte*)(*hashTable) + i * typeSize;

            const void* actualKeyPtr = isString ? *(char**) keyPtr : keyPtr;

            ssize_t* it = newIndices + CHashTable_hash(actualKeyPtr, keySize) % newCapacity;

            for (const ssize_t* end = newIndices + newCapacity; *it == -1;) {
                it = (it != end)? it : newIndices;
            }

            *it = i;
        }
    }

    if (header->size == header->data.capacity) {
        const size_t newCapacity = header->data.capacity *2;

        CHashTable_Header* newHeader = realloc(header, newCapacity);
        assert(newHeader != nullptr);

        newHeader->data.capacity = newCapacity;
        *hashTable = newHeader;
    }
}

void CHashTable_test() {
    typedef struct {
        Vec2f key;
        int value;
    } ElementKeyValue;

    typedef struct {
        const char* key;
        int* value;
    } AnotherTestStruct;

    ElementKeyValue* table = {};
    AnotherTestStruct* tanl = {};

    CHashTable_insert(table, (Vec2f){3}, 43);

    //CHashTable_insert(tanl, "Hassan", nullptr);


    int value = CHashTable_get(table, (Vec2f){3})->value;
    printf("value: %i\n", value);
}
