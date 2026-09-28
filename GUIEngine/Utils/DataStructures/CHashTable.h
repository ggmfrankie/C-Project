//
// Created by Stefan on 27.09.2026.
//

#pragma once
#include <stdint.h>

#include "Utils/Typedef.h"

#define CHASH_TABLE_INIT_CAPACITY 16

typedef struct {
    struct {
        ssize_t* data;
        size_t capacity;
    } table;

    struct {
        size_t capacity;
    } data;

    size_t size;

} CHashTable_Header;

#define _CHashTable_getHeader(hashTable) (&((CHashTable_Header*)(hashTable))[-1])

#define _CHashTable_typeIdentity(type, arg) _Generic((arg), type: (arg), default: (type)0)

#define _CHashTable_isString(type) _Generic((type), char*: true, const char*: true, default: false)
#define _CHashTable_getKeySize(key)\
    _Generic((key),\
        char* : strlen(_CHashTable_typeIdentity(char*, (key))),\
        const char* : strlen(_CHashTable_typeIdentity(const char*, (key))),\
        default: sizeof((key))\
    )
#define _CHashTable_getKeyAddress(key)\
(byte*)_Generic((key),\
        char* : (key),\
        const char* : (key),\
        default: &(key)\
    )

void _CHashTable_new(void** table, size_t typeSize, size_t capacity);
void *_CHashTable_get(void *hashTable, const byte *keyData, size_t keySize, size_t typeSize, bool isString);
ssize_t* _CHashTable_getFreeIndex(void* hashTable, const byte* keyData, size_t keySize);
void _CHashTable_growIfNeeded(void** hashTable, uint32_t typeSize, uint32_t keySize, bool isString);

#define CHashTable_insert(table, key, value)\
do {\
    if ((table) == nullptr) _CHashTable_new((void**)(&table), sizeof(*(table)), CHASH_TABLE_INIT_CAPACITY);\
    const size_t keySize = _CHashTable_getKeySize(key);\
\
    _CHashTable_growIfNeeded((void**)(&table), sizeof(*(table)), keySize, _CHashTable_isString(key));\
\
    const size_t index = _CHashTable_getHeader(table)->size++;\
    (table)[index] = (typeof(*(table))){key, value};\
\
    const byte* keyAddress = _CHashTable_getKeyAddress(key);\
\
    *_CHashTable_getFreeIndex((table), keyAddress, keySize) = index;\
} while (0)

#define CHashTable_get(table, key) ((typeof(table)) _CHashTable_get((table), _CHashTable_getKeyAddress(key), sizeof(key), sizeof(*(table)), _CHashTable_isString(key)))

void _CHashTable_free(void** table);
#define CHashTable_free(table) _CHashTable_free((void**)(&table))

void CHashTable_test();