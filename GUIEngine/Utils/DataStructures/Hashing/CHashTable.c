//
// Created by Stefan on 27.09.2026.
//

#include "CHashTable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../CInlineStack.h"
#include "../String/CStr.h"
#include "Utils/Macros/Defer.h"
#include "Utils/Math/Vector.h"

#define CHashTable_Seed 342341431UL

static void CHashTable_printHeader(const CHashTable_Header* header) {
    printf("Header:\n  {data = %p; capacity = %llu;} table\n {capacity = %llu;} data\n size = %llu\n", header->table.data, header->table.capacity, header->data.capacity, header->size);
}

bool CHashTable_memoryComp(const void* keyPtrA, const void* keyPtrB, size_t keySize) {
    return memcmp(keyPtrA, keyPtrB, keySize) == 0;
}

bool CHashTable_stringComp(const void* keyPtrA, const void* keyPtrB, size_t) {
    return cstrEquals(*(char**)keyPtrA, *(char**)keyPtrB);
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

uint32_t CHashTable_memoryHash(const byte* key, size_t len) {
    return CHashTable_hash(key, len);
}

uint32_t CHashTable_stringHash(const byte* key, size_t) {
    return CHashTable_hash((byte*)*(char**)key, strlen(*(char**)key));
}

void _CHashTable_new(void* table[], size_t capacity, size_t typeSize, size_t keySize, size_t keyOffset, CHashTable_keyComparator keyComparator, CHashTable_keyHash keyHash) {
    CHashTable_Header* header = malloc(sizeof(CHashTable_Header) + typeSize * capacity);
    assert(header != nullptr);

    header->data.capacity = capacity;
    header->data.typeSize = typeSize;
    header->key.size = keySize;
    header->size = 0;

    const size_t tableCapacity = capacity * 4;
    ssize_t* indices = malloc(sizeof(*indices) * tableCapacity);
    assert(indices != nullptr);
    memset(indices, -1, tableCapacity * sizeof(*indices));

    header->table.data = indices;
    header->table.capacity = tableCapacity;

    header->comparator = keyComparator;
    header->hash = keyHash;
    header->key.offset = keyOffset;

    *table = (void*) (header+1);
}

static byte* CHashTable_getDataSlot(void* hashTable, size_t index) {
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);
    return (byte*) hashTable + index * header->data.typeSize;
}

static ssize_t* CHashTable_getHashIndexSlot(void* hashTable, const byte* keyPtr) {
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    const size_t capacity = header->table.capacity;

    const size_t keySize = header->key.size;

    ssize_t* indices = header->table.data;

    const ssize_t index = header->hash(keyPtr, keySize) % capacity;
    ssize_t* it = &indices[index];

    const ssize_t* end = &indices[capacity];

    const CHashTable_keyComparator comparator = header->comparator;

    while (*it != -1) {
        // Key at the iterator equals the provided key
        const void* keySlot = CHashTable_getDataSlot(hashTable, *it);
        if (comparator(keySlot, keyPtr, keySize)) return it;
        ++it;
        // Wrap around at the end
        if(it == end) it = indices;
    }
    return nullptr;
}

void* _CHashTable_get(void* hashTable, const byte* keyPtr) {
    assert(hashTable != nullptr && keyPtr != nullptr);

    const ssize_t* hashIndex = CHashTable_getHashIndexSlot(hashTable, keyPtr);

    return hashIndex
        ? CHashTable_getDataSlot(hashTable, *hashIndex)
        : nullptr;
}

ssize_t* _CHashTable_getFreeHashIndexSlot(void* hashTable, const byte *keyData) {
    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    ssize_t* indices = header->table.data;
    const size_t capacity = header->table.capacity;
    const size_t keySize = header->key.size;

    const ssize_t index = header->hash(keyData, keySize) % capacity;

    ssize_t* it = &indices[index];
    const ssize_t* end = &indices[capacity];

    while (*it != -1) {
        ++it;
        if(it == end) it = indices;
    }
    return it;
}

static void CHashTable_rehash(void* table[]) {
    const CHashTable_Header* header = _CHashTable_getHeader(*table);

    const size_t capacity = header->table.capacity;
    ssize_t* indices = header->table.data;

    memset(indices, -1, capacity * sizeof(*indices));

    // Iterate over the data array and rehash all the entries
    for (int i = 0; i < header->size; ++i) {
        const void* keyPtr = CHashTable_getDataSlot(*table, i) + header->key.offset;
        *_CHashTable_getFreeHashIndexSlot(*table, keyPtr) = i;
    }
}

void _CHashTable_growIfNeeded(void* table[]) {
    CHashTable_Header* header = _CHashTable_getHeader(*table);

    if ((float)header->size / header->table.capacity > 0.75) {
        const size_t newCapacity = header->table.capacity *2;

        ssize_t* newIndices = realloc(header->table.data, newCapacity * sizeof(*newIndices));
        assert(newIndices != nullptr);

        header->table.data = newIndices;
        header->table.capacity = newCapacity;

        CHashTable_rehash(table);
    }

    // Handle growing the data array
    if (header->size == header->data.capacity) {
        const size_t newCapacity = header->data.capacity *2;

        CHashTable_Header* newHeader = realloc(header, sizeof(CHashTable_Header) + newCapacity * header->data.typeSize);
        assert(newHeader != nullptr);

        newHeader->data.capacity = newCapacity;
        *table = (void*)(newHeader + 1);
    }
}

void _CHashTable_free(void **table) {
    if (*table == nullptr) return;
    free(_CHashTable_getHeader(*table));
    *table = nullptr;
}

void _CHashTable_remove(void* table[], const byte* keyData) {
    CHashTable_Header* header = _CHashTable_getHeader(*table);

    const size_t size = header->size;
    const size_t capacity = header->table.capacity;
    const size_t keyOffset = header->key.offset;

    ssize_t* indices = header->table.data;

    // First: find the element that needs to be deleted
    ssize_t* slot = CHashTable_getHashIndexSlot(*table, keyData);

    if (slot == nullptr || *slot == -1) return;

    const ssize_t index = *slot;

    //--------------------------------------------------------------------------------//
    // Handle possible probe inserted elements
    {
        const ssize_t* hashTableEnd = indices + capacity;
        // Deletion of the hash index inside the index array
        ssize_t* probeStart = slot + 1;
        if (probeStart == hashTableEnd) probeStart = indices;
        ssize_t* probeScan = probeStart;

        // Need move possible probing blob
        size_t blockSize = 0;

        // Get the block size
        while (*probeScan != -1) {
            // Handel wrapping
            ++blockSize;

            ++probeScan;
            if (probeScan == hashTableEnd) probeScan = indices;
        }

        const ssize_t* probeEnd = probeScan;
        probeScan = probeStart;

        size_t rehashStack[blockSize];
        size_t rehashStackSize = 0;

        // Store indices of elements that need rehashing
        while (probeScan != probeEnd) {
            rehashStack[rehashStackSize++] = *probeScan;
            *probeScan = -1;

            ++probeScan;
            if (probeScan == hashTableEnd) probeScan = indices;
        }

        for (size_t i = 0; i < rehashStackSize; ++i) {
            const size_t dataIndex = rehashStack[i];
            const void* keyPtr = CHashTable_getDataSlot(*table, dataIndex) + keyOffset;
            ssize_t* foundHashTableSlot = _CHashTable_getFreeHashIndexSlot(*table, keyPtr);

            *foundHashTableSlot = dataIndex;
        }
    }

    //--------------------------------------------------------------------------------//
    // Deletion of the actual element inside the data array
    {
        if (index < size-1) {
            void* deleteSlot = CHashTable_getDataSlot(*table, index);
            memcpy(deleteSlot, CHashTable_getDataSlot(*table, size-1), header->data.typeSize);

            const void* lastKey = deleteSlot + keyOffset;

            ssize_t* it = CHashTable_getHashIndexSlot(*table, lastKey);

            *it = index;
        }

        header->size--;
    }
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

    //int value = CHashTable_get(table, (Vec2f){3})->value;

    for CHashTable_each(ach, tanl) {
        printf("%s\n", ach->key);
    }

    printf("value: n");
}
