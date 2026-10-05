//
// Created by Stefan on 27.09.2026.
//

#include "CHashTable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "CStr.h"
#include "Utils/Logging/Logging.h"
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

void* _CHashTable_get(void* hashTable, const byte* keyPtr) {
    assert(hashTable != nullptr && keyPtr != nullptr);

    const CHashTable_Header* header = _CHashTable_getHeader(hashTable);

    const ssize_t* indices = header->table.data;
    const size_t capacity = header->table.capacity;

    const size_t typeSize = header->data.typeSize;
    const size_t keySize = header->key.size;
    const size_t keyOffset = header->key.offset;

    const CHashTable_keyComparator comparator = header->comparator;

    const ssize_t index = header->hash(keyPtr, keySize) % capacity;

    const ssize_t* it = &indices[index];

    const ssize_t* end = &indices[capacity];

    while (*it != -1) {
        // Key at the iterator equals the provided key
        void* keySlot = (byte*) hashTable + ((*it) * typeSize) + keyOffset;
        if (comparator(keySlot, keyPtr, keySize)) return keySlot;
        ++it;
        // Wrap around at the end
        if(it == end) it = indices;
    }
    Log_trace("Value not inside hashtable");
    return nullptr;
}

ssize_t* _CHashTable_getFreeIndex(void* hashTable, const byte *keyData) {
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

void _CHashTable_growIfNeeded(void* table[]) {
    CHashTable_Header* header = _CHashTable_getHeader(*table);

    if ((float)header->size / header->table.capacity > 0.75) {
        const size_t newCapacity = header->table.capacity *2;

        ssize_t* newIndices = realloc(header->table.data, newCapacity * sizeof(*newIndices));
        assert(newIndices != nullptr);
        memset(newIndices, -1, newCapacity * sizeof(*newIndices));

        header->table.data = newIndices;
        header->table.capacity = newCapacity;

        const size_t keySize = header->key.size;
        const size_t keyOffset = header->key.offset;
        const size_t typeSize = header->data.typeSize;

        const CHashTable_keyHash hash = header->hash;

        // Iterate over the data array and rehash all the entries
        for (int i = 0; i < header->size; ++i) {
            const void* keyPtr = (byte*) (*table) + i * typeSize + keyOffset;

            const size_t hashIndex = hash(keyPtr, keySize) % newCapacity;

            ssize_t* it = newIndices + hashIndex;

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
    const size_t keySize = header->key.size;
    const size_t keyOffset = header->key.offset;
    const size_t typeSize = header->data.typeSize;

    ssize_t* indices = header->table.data;

    const CHashTable_keyHash hash = header->hash;
    const CHashTable_keyComparator comparator = header->comparator;

    // First: find the element that needs to be deleted
    const ssize_t hashIndex  = hash((byte*)keyData, keySize) % capacity;

    ssize_t* slot = &header->table.data[hashIndex];
    const ssize_t* end = indices + capacity;

    ssize_t index = -1;
    while (*slot != -1) {
        const void* keySlot = (byte*)(*table) + ((*slot) * typeSize) + keyOffset;
        if (comparator(keySlot, keyData, keySize)) {
            index = *slot;
            break;
        }
        ++slot;
        // Wrap around at the end
        if (slot == end) slot = indices;
    }
    if (index == -1) return;

    ssize_t* probeSlot = slot + 1;

    if (probeSlot == end) probeSlot = indices;

    ssize_t* probeScan = probeSlot;

    // Need move possible probing blob
    while (*probeScan != -1) {
        // Handel wrapping
        if (probeScan == end) {
            if (probeScan - probeSlot == 0) break;

            memmove(probeSlot, probeSlot + 1, (probeScan - probeSlot) * sizeof(*slot));
            *(probeScan-1) = -1;

            probeSlot = indices;
            probeScan = indices;
        }

        const size_t probeHashIndex = hash((byte*)(*table) + ((*slot) * typeSize) + keyOffset, keySize) % capacity;
        // Move probe forward if the item referenced shares the same hash key
        if (probeHashIndex == hashIndex) {
            probeScan++;
        } else {
            if (probeScan - probeSlot == 0) break;
            // Move the blob one over
            memmove(probeSlot, probeSlot + 1, (probeScan - probeSlot) * sizeof(*slot));
            *(probeScan-1) = -1;
            probeSlot = probeScan;
        }
    }

    // Else we can just pop the back
    if (index < size-1) {
        memcpy((byte*)(*table) + index * typeSize, (byte*)(*table) + (size-1) * typeSize, header->data.typeSize);

        const void* lastKey = (*table) + index * typeSize + keyOffset;
        const size_t lastHashIndex = hash(lastKey, keySize) % capacity;

        ssize_t* it = &header->table.data[lastHashIndex];

        while (*it != -1) {
            const void* keySlot = (byte*)(*table) + ((*it) * typeSize) + keyOffset;
            if (comparator(keySlot, lastKey, keySize)) {
                *it = index;
                goto PopLast;
            }
            ++it;
            // Wrap around at the end
            if(it == end) it = indices;
        }
    }
    PopLast:
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

    do {
        if ((tanl) == nullptr) _CHashTable_new((void **) (&tanl), CHASH_TABLE_INIT_CAPACITY, sizeof(*(tanl)),
                                               sizeof((tanl)->key), offsetof(typeof(*tanl), key),
                                               _CHashTable_getKeyComparator((tanl)->key),
                                               _CHashTable_getKeyHash((tanl)->key));
        _CHashTable_growIfNeeded((void **) (&tanl));
        const size_t index = _CHashTable_getHeader(tanl)->size++;
        (tanl)[index] = (typeof(*(tanl))){"Hassan", ((ElementKeyValue){.key = (Vec2f){0.0, 1.69}})};
        auto key = "Hassan";
        const auto keyPtr = &key;
        *_CHashTable_getFreeIndex((tanl), (byte *) (keyPtr)) = index;
    } while (0);

    //int value = CHashTable_get(table, (Vec2f){3})->value;
    volatile auto value2 = CHashTable_get(tanl, "Hassan")->value.key;

    for CHashTable_each(ach, tanl) {
        printf("%s\n", ach->key);
    }

    printf("value: n");
}
