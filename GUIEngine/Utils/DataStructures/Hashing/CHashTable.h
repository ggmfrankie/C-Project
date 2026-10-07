//
// Created by Stefan on 27.09.2026.
//

#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Utils/Typedef.h"
#include "../../Macros/Macros.h"


#define CHASH_TABLE_SHORT_NAMES
#ifdef CHASH_TABLE_SHORT_NAMES

#define htInsert CHashTable_insert
#define htGet    CHashTable_get
#define htLen    CHashTable_len
#define htFree   CHashTable_free

#define htEach   CHashTable_each

#endif


#define CHASH_TABLE_INIT_CAPACITY 16

typedef bool (*CHashTable_keyComparator)(const void* keyPtrA, const void* keyPtrB, size_t keySize);
typedef uint32_t (*CHashTable_keyHash)(const byte* key, size_t len);

typedef struct {
    CHashTable_keyHash hash;
    CHashTable_keyComparator comparator;

    struct {
        size_t size;
        size_t offset;
    } key;

    struct {
        ssize_t* data;
        size_t capacity;
    } table;

    struct {
        size_t capacity;
        size_t typeSize;
    } data;

    size_t size;
} CHashTable_Header;

void _CHashTable_new(
    void* table[],
    size_t capacity,

    size_t typeSize,
    size_t keySize,
    size_t keyOffset,
    CHashTable_keyComparator keyComparator,
    CHashTable_keyHash keyHash
);

void _CHashTable_free(void** table);

void _CHashTable_remove(
    void* table[],
    const byte* keyData
);

void _CHashTable_growIfNeeded(
    void* table[]
);

void* _CHashTable_get(
    void *hashTable,
    const byte *keyPtr
);

ssize_t* _CHashTable_getFreeHashIndexSlot(
    void* hashTable,
    const byte* keyData
);

bool CHashTable_memoryComp(const void* keyPtrA, const void* keyPtrB, size_t keySize);
bool CHashTable_stringComp(const void* keyPtrA, const void* keyPtrB, size_t);
uint32_t CHashTable_memoryHash(const byte* key, size_t len);
uint32_t CHashTable_stringHash(const byte* key, size_t);

#define _CHashTable_getHeader(pHashTable) (&((CHashTable_Header*)(pHashTable))[-1])

#define _CHashTable_getKeyComparator(pKey)\
_Generic((pKey),\
    char* :       CHashTable_stringComp,\
    const char* : CHashTable_stringComp,\
    default:      CHashTable_memoryComp\
)

#define _CHashTable_getKeyHash(pKey)\
_Generic((pKey),\
    char* :       CHashTable_stringHash,\
    const char* : CHashTable_stringHash,\
    default:      CHashTable_memoryHash\
)

#define CHashTable_insert(pTable, pKey, pValue)\
do {\
    if ((pTable) == nullptr) _CHashTable_new((void**)(&pTable), CHASH_TABLE_INIT_CAPACITY, sizeof(*(pTable)), sizeof((pTable)->key), offsetof(typeof(*pTable), key), _CHashTable_getKeyComparator((pTable)->key), _CHashTable_getKeyHash((pTable)->key));\
\
    _CHashTable_growIfNeeded((void**)(&pTable));\
\
    const size_t index = _CHashTable_getHeader(pTable)->size++;\
    (pTable)[index] = (typeof(*(pTable))){pKey, pValue};\
\
    auto key = pKey;\
    *_CHashTable_getFreeHashIndexSlot((pTable), (byte*) &(key)) = index;\
} while (0)

#define CHashTable_get(pTable, pKey) ({auto key = pKey; (typeof(pTable)) _CHashTable_get((pTable), (byte*)&(key));})

#define CHashTable_len(pTable) ((pTable)?_CHashTable_getHeader(pTable)->size:0)
#define CHashTable_isEmpty(pTable) ((pTable)?CHashTable_len(pTable)==0:true)

#define CHashTable_free(pTable) _CHashTable_free((void**)(&pTable))
#define CHashTable_remove(pTable, pKey) do {auto key = pKey; _CHashTable_remove((void**)(&pTable), (byte*)&(key));} while(0)

#define CHashTable_each_impl(_end, pItem, pTable) (typeof(*(pTable))* pItem = (pTable), *_end = (pTable) + CHashTable_len(pTable); (pItem) != _end; ++(pItem))
#define CHashTable_each(pItem, pTable) CHashTable_each_impl(CONCAT(_end, __COUNTER__), pItem, pTable)

void CHashTable_test();