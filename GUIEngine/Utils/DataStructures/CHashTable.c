//
// Created by Stefan on 27.09.2026.
//

#include "CHashTable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CStr.h"
#include "Utils/Logging/Logging.h"
#include "Utils/Macros/Defer.h"
#include "Utils/Math/Vector.h"

#define CHashTable_Seed 342341431UL

static void CHashTable_printHeader(const CHashTable_Header* header) {
    printf("Header:\n  {data = %p; capacity = %llu;} table\n {capacity = %llu;} data\n size = %llu\n", header->table.data, header->table.capacity, header->data.capacity, header->size);
}

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

void _CHashTable_new(void* table[], size_t typeSize, size_t capacity) {
    CHashTable_Header* header = malloc(sizeof(CHashTable_Header) + typeSize * capacity);
    assert(header != nullptr);

    header->data.capacity = capacity;
    header->size = 0;

    const size_t tableCapacity = capacity * 4;
    ssize_t* indices = malloc(sizeof(*indices) * tableCapacity);
    assert(indices != nullptr);
    memset(indices, -1, tableCapacity * sizeof(*indices));

    header->table.data = indices;
    header->table.capacity = tableCapacity;

    *table = (void*) (header+1);
}

void* _CHashTable_get(void* hashTable, const byte* keyData, size_t keySize, size_t typeSize, bool isString) {
    assert(hashTable != nullptr && keyData != nullptr);
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    const ssize_t* indices = header->table.data;
    const size_t capacity = header->table.capacity;

    const ssize_t index = CHashTable_hash(keyData, keySize) % capacity;

    const ssize_t* it = indices + index;

    const ssize_t* end = indices + capacity;

    while (*it != -1) {
        // Key at the iterator equals the provided key
        void* slot = (byte*)hashTable + ((*it) * typeSize);
        if ((isString)
            ? cstrEquals(*(char**)slot, (char*)keyData)
            : memcmp(slot, keyData, keySize) == 0) return slot;
        ++it;
        // Wrap around at the end
        if(it == end) it = indices;
    }
    Log_trace("Value not inside hashtable");
    return nullptr;
}

ssize_t* _CHashTable_getFreeIndex(void* hashTable, const byte *keyData, size_t keySize) {
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    ssize_t* indices = header->table.data;
    const size_t capacity = header->table.capacity;

    const ssize_t index = CHashTable_hash(keyData, keySize) % capacity;

    ssize_t* it = indices + index;
    const ssize_t* end = indices + capacity;

    while (*it != -1) {
        ++it;
        if(it == end) it = indices;
    }
    return it;
}

void _CHashTable_growIfNeeded(void* hashTable[], uint32_t typeSize, uint32_t keySize, bool isString) {
    CHashTable_Header* header = _CHashTable_getHeader(*hashTable);

    if ((float)header->size / header->table.capacity > 0.75) {
        const size_t newCapacity = header->table.capacity *2;

        ssize_t* newIndices = realloc(header->table.data, newCapacity * sizeof(*newIndices));
        assert(newIndices != nullptr);
        memset(newIndices, -1, newCapacity * sizeof(*newIndices));

        header->table.data = newIndices;
        header->table.capacity = newCapacity;

        /*
         * Iterate over the data array and rehash all the entries
         */
        for (int i = 0; i < header->size; ++i) {
            const void* keyPtr = (byte*)(*hashTable) + i * typeSize;

            const void* keyData = isString ? *(char**) keyPtr : keyPtr;
            const int size = isString ? strlen(keyData) : keySize;

            const size_t index = CHashTable_hash(keyData, size) % newCapacity;

            ssize_t* it = newIndices + index;

            const ssize_t* end = newIndices + newCapacity;

            while (*it != -1) {
                ++it;
                if(it == end) it = newIndices;
            }

            *it = i;
        }
    }

    // Handle growing the data array
    if (header->size == header->data.capacity) {
        const size_t newCapacity = header->data.capacity *2;

        CHashTable_Header* newHeader = realloc(header, sizeof(CHashTable_Header) + newCapacity * typeSize);
        assert(newHeader != nullptr);

        newHeader->data.capacity = newCapacity;
        *hashTable = (void*)(newHeader + 1);
    }
}

void _CHashTable_free(void **table) {
    if (*table == nullptr) return;
    free(_CHashTable_getHeader(*table));
    *table = nullptr;
}

void _CHashTable_remove(void* table[], const byte* keyData, size_t keySize, size_t typeSize, bool isString) {
    CHashTable_Header* header = _CHashTable_getHeader(*table);
    const size_t size = header->size;
    const size_t capacity = header->table.capacity;
    const ssize_t tableIndex = CHashTable_hash(keyData, keySize) % capacity;

    ssize_t* slot = &header->table.data[tableIndex];
    if (*slot == -1) return;

    if (*slot < size) {
        memcpy((byte*)(*table) + *slot, (byte*)(*table) + size, typeSize);
        // TODO: fix deletion not updating the indices inside the table for the last element;
    }

    *slot = -1;
    header->size--;
}

void CHashTable_test() {
    typedef struct {
        Vec2f key;
        int value;
    } ElementKeyValue;

    typedef struct {
        const char* key;
        ElementKeyValue value;
    } AnotherTestStruct;

    defer(defer_CHashTableFree) ElementKeyValue* table = {};
    defer(defer_CHashTableFree) AnotherTestStruct* tanl = {};

    //CHashTable_insert(table, (Vec2f){3}, 43);

    CHashTable_insert(tanl, "Hassan", ((ElementKeyValue){.key = (Vec2f){0.0, 1.69}}));


    //int value = CHashTable_get(table, (Vec2f){3})->value;
    volatile auto value2 = CHashTable_get(tanl, "Hassan")->value.key;

    for CHashTable_each(ach, tanl) {
        printf("%s\n", ach->key);
    }

    printf("value: n");
}
